#include "BMP.hpp"

auto BMP::encodeMessageToFile() -> void {
    if(!containMessage()) {
        fmt::println("Message is too big to write in this file.");
        return;
    }

    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::out | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read or write to the file");
        exit(1);
    }

    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIBHeader));

    auto messageBinary = messageToBinary(getMessage());
    auto messageIndex = 0;

    auto codeMessage = [&messageIndex, messageBinary](auto &color) -> void {
        if (messageIndex < messageBinary.size() && (color & 1) != messageBinary[messageIndex++]) {
            auto lsb = color & 1;
            color = ((color >> 1) << 1) | !lsb;
        }
    };

    auto pixelsArraySize = messageBinary.size();
    auto pixelsArray = std::vector<uint8_t>(pixelsArraySize);

    file.seekg(bmp_header.dataOffset, std::ios::beg);

    file.read(reinterpret_cast<char*>(pixelsArray.data()), pixelsArraySize);

    for (auto &color: pixelsArray) {
        codeMessage(color);
    }

    file.seekp(bmp_header.dataOffset, std::ios::beg);
    file.write(reinterpret_cast<char const*>(pixelsArray.data()), pixelsArraySize);

    file.close();

    fmt::println("Message encoded successfully");
}

auto BMP::decodeMessageFromFile() -> void {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIBHeader));

    auto rowSize = int(ceil(dib_header.bitsPerPixel * dib_header.width / 32.0) * 4);

    auto pixlesArraySize = rowSize * abs(dib_header.height);

    auto pixelsArray = std::vector<uint8_t>(pixlesArraySize);

    file.seekg(bmp_header.dataOffset);
    file.read(reinterpret_cast<char*>(pixelsArray.data()), pixlesArraySize);

    file.close();

    auto LSBArray = std::vector<int>();

    for (auto &color: pixelsArray) {
        LSBArray.push_back(getLSB(color));
    }

    readDecodedMessage(LSBArray);
}

auto BMP::getFileInfo() -> void {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIBHeader));

    file.close();

    auto fileName = std::filesystem::path(getFilePath()).filename().string();
    auto fileSize = std::filesystem::file_size(getFilePath());

    fmt::println("File format: BMP\n");

    fmt::println(
            "The BMP file format or bitmap, is a raster graphics image file format used to store bitmap digital images, independently of the display device (such as a graphics adapter)");
    fmt::println("Wikipedia: https://en.wikipedia.org/wiki/BMP_file_format\n");

    fmt::println("Image \"{}\" properties:\n", fileName);

    fmt::println("Height: {}", dib_header.height);
    fmt::println("Width: {}", dib_header.width);
    fmt::println("Size: {}B", fileSize);

    /*
     * https://en.cppreference.com/w/cpp/filesystem/last_write_time
     */
    auto timeStamp = std::filesystem::last_write_time(getFilePath());
    // formatuje wedlug kodu ze ww. strony
    fmt::println("{}", std::format("Last modified: {}\n", timeStamp));
}

auto BMP::containMessage() -> bool {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    BMPHeader bmp_header{};
    DIBHeader dib_header{};

    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(BMPHeader));
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(DIBHeader));

    auto width = dib_header.width;
    auto height = dib_header.height;

    auto possibleBits = width * height * dib_header.bitsPerPixel;
    auto messageSize = getMessage().size() * 8 + 16; // Size of message + 2 size of 2 special characters

    return messageSize <= possibleBits;
}
