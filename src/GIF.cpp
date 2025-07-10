#include "GIF.hpp"

auto GIF::encodeMessageToFile() -> void {
    // https://www.fileformat.info/format/gif/egff.htm

    //testing if message is small enought to encode in this file
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

    GIFHeaderAndLogicalScreenDescriptor gifBasicData{};

    file.read(reinterpret_cast<char*>(&gifBasicData), sizeof(GIFHeaderAndLogicalScreenDescriptor));

    auto sizeOfGlobalColorTable = gifBasicData.packed & 0b00000111;
//    auto colorTableSortFlag = (gifBasicData.packed >> 3) & 0b1;
//    auto colorResolution = (gifBasicData.packed >> 4) & 0b0111;
    auto globalColorTableFlag = (gifBasicData.packed >> 7) & 0b1;

    if (globalColorTableFlag == 1) {
        auto colorTableSize = 3L * (1L << (sizeOfGlobalColorTable + 1));

        auto dataOffset = file.tellg();

        auto globalColorTable = std::vector<uint8_t>(colorTableSize);
        file.read(reinterpret_cast<char*>(globalColorTable.data()), colorTableSize);

        auto messageBinary = messageToBinary(getMessage());
        auto messageIndex = 0;

        auto codeMessage = [&messageIndex, messageBinary](auto &color) -> void {
            if (messageIndex < messageBinary.size() && (color & 1) != messageBinary[messageIndex++]) {
                auto lsb = color & 1;
                color = ((color >> 1) << 1) | !lsb;
            }
        };

        for (auto &color: globalColorTable) {
            codeMessage(color);
        }

        file.seekp(dataOffset);

        file.write(reinterpret_cast<char const*>(globalColorTable.data()), colorTableSize);

        file.close();
    }

    fmt::println("Message encoded successfully");
}

auto GIF::decodeMessageFromFile() -> void {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    GIFHeaderAndLogicalScreenDescriptor gifBasicData{};

    file.read(reinterpret_cast<char*>(&gifBasicData), sizeof(GIFHeaderAndLogicalScreenDescriptor));

    auto sizeOfGlobalColorTable = gifBasicData.packed & 0b00000111;
//    auto colorTableSortFlag = (gifBasicData.packed >> 3) & 0b1;
//    auto colorResolution = (gifBasicData.packed >> 4) & 0b0111;
    auto globalColorTableFlag = (gifBasicData.packed >> 7) & 0b1;

    if (globalColorTableFlag == 1) {
        auto colorTableSize = 3L * (1L << (sizeOfGlobalColorTable + 1));

        auto globalColorTable = std::vector<uint8_t>(colorTableSize);
        file.read(reinterpret_cast<char*>(globalColorTable.data()), colorTableSize);

        auto LSBArray = std::vector<int>();

        for (auto &color: globalColorTable) {
            LSBArray.push_back(getLSB(color));
        }

        readDecodedMessage(LSBArray);
    }
}

auto GIF::getFileInfo() -> void {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    GIFHeaderAndLogicalScreenDescriptor gifBasicData{};

    file.read(reinterpret_cast<char*>(&gifBasicData), sizeof(GIFHeaderAndLogicalScreenDescriptor));

    file.close();

    auto fileName = std::filesystem::path(getFilePath()).filename().string();
    auto fileSize = std::filesystem::file_size(getFilePath());

    fmt::println("File format: GIF\n");

    fmt::println(
            "The Graphics Interchange Format (GIF) is a bitmap image format that was developed by a team at the online services provider CompuServe led by American computer scientist Steve Wilhite and released on June 15, 1987");
    fmt::println("Wikipedia: https://en.wikipedia.org/wiki/GIF\n");

    fmt::println("Image \"{}\" properties:\n", fileName);

    fmt::println("Version: {:c}{:c}{:c}", gifBasicData.version[0], gifBasicData.version[1], gifBasicData.version[2]);
    fmt::println("Width: {}", gifBasicData.screenWidth);
    fmt::println("Height: {}", gifBasicData.screenHeight);
    fmt::println("Size: {}B", fileSize);
    fmt::println("Background color: {}", gifBasicData.backgroundColor);
    fmt::println("Aspect ratio: {}", gifBasicData.aspectRatio);

    /*
    * https://en.cppreference.com/w/cpp/filesystem/last_write_time
    */
    auto timeStamp = std::filesystem::last_write_time(getFilePath());
    // formatuje wedlug kodu ze ww. strony
    fmt::println("{}", std::format("Last modified: {}", timeStamp));

}

auto GIF::containMessage() -> bool {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.good()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    //https://www.fileformat.info/format/gif/egff.htm

    auto messageSize = getMessage().size() * 8 + 16; // Size of message + 2 size of 2 special characters

    return messageSize <= 768;
}
