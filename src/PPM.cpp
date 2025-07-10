#include <iostream>
#include <fstream>
#include <vector>
#include <fmt/core.h>
#include <string>
#include "utils.hpp"
#include "PPM.hpp"
#include <filesystem>
#include <chrono>

auto PPM::encodeMessageToFile() -> void {
    /*
     * https://en.wikipedia.org/wiki/Netpbm#File_formats
     * https://paulbourke.net/dataformats/ppm/
     */

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

    auto magicNumber = std::string();
    auto commentsBeforeSize = std::vector<std::string>();
    auto size = std::string();
    auto commentsAfterSize = std::vector<std::string>();
    auto maxValString = std::string();

    std::getline(file, magicNumber);
    readComments(file, commentsBeforeSize);

    std::getline(file, size);

    PPM::readComments(file, commentsAfterSize);

    std::getline(file, maxValString);

    auto messageBinary = messageToBinary(getMessage());
    auto messageIndex = 0;

    auto codeMessage = [&messageIndex, messageBinary](auto &color) -> void {
        if (messageIndex < messageBinary.size() && (color & 1) != messageBinary[messageIndex++]) {
            auto lsb = color & 1;
            color = ((color >> 1) << 1) | !lsb;
        }
    };

    auto pixelsArraySize = messageBinary.size();

    // miejsce w ktorym zacyznaja sie dane
    auto dataOffset = file.tellg();

    if (magicNumber == "P6") {

        auto pixelsArray = std::vector<uint8_t>(pixelsArraySize);

        file.read(reinterpret_cast<char*>(pixelsArray.data()), pixelsArraySize);

        for (auto &color: pixelsArray) {
            codeMessage(color);
        }

        file.seekp(dataOffset);

        file.write(reinterpret_cast<char const*>(pixelsArray.data()), pixelsArraySize);
    } else if (magicNumber == "P3") {

        auto pixelsArray = std::vector<uint8_t>();

        for (auto i = 0; i < pixelsArraySize; i++) {
            auto color = std::string();
            std::getline(file, color);
            auto colorValue = std::stoi(color);
            pixelsArray.push_back(colorValue);
        }

        for (auto &color: pixelsArray) {
            codeMessage(color);
        }

        file.seekp(dataOffset);

        for (auto &color: pixelsArray) {
            file << fmt::format("{}\n", color);
        }

    }

    file.close();

    fmt::println("Message encoded successfully");
}

auto PPM::decodeMessageFromFile() -> void {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.is_open()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    auto magicNumber = std::string();
    auto commentsBeforeSize = std::vector<std::string>();
    auto size = std::string();
    auto commentsAfterSize = std::vector<std::string>();
    auto maxValString = std::string();

    std::getline(file, magicNumber);
    readComments(file, commentsBeforeSize);

    std::getline(file, size);

    // https://stackoverflow.com/questions/54113125/c-how-to-convert-substring-to-int
    auto width = std::stoi(size.substr(0, size.find(' ')));
    auto height = std::stoi(size.substr(size.find(' ') + 1));
    readComments(file, commentsAfterSize);

    std::getline(file, maxValString);

    auto pixelsArraySize = width * height * 3;
    auto pixelsArray = std::vector<uint8_t>();

    if (magicNumber == "P6") {
        pixelsArray = std::vector<uint8_t>(pixelsArraySize);

        file.read(reinterpret_cast<char*>(pixelsArray.data()), pixelsArraySize);
    } else if (magicNumber == "P3") {
        for (auto i = 0; i < pixelsArraySize; i++) {
            auto color = std::string();
            std::getline(file, color);
            auto colorValue = std::stoi(color);
            pixelsArray.push_back(colorValue);
        }
    }

    file.close();

    auto LSBArray = std::vector<int>();

    for (auto &color: pixelsArray) {
        LSBArray.push_back(getLSB(color));
    }

    readDecodedMessage(LSBArray);

}

auto PPM::getFileInfo() -> void {
    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    if(!file.good()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    auto magicNumber = std::string();
    auto commentsBeforeSize = std::vector<std::string>();
    auto size = std::string();
    auto commentsAfterSize = std::vector<std::string>();
    auto maxValString = std::string();

    std::getline(file, magicNumber);
    readComments(file, commentsBeforeSize);

    std::getline(file, size);

    auto width = std::stoi(size.substr(0, size.find(' ')));
    auto height = std::stoi(size.substr(size.find(' ') + 1));

    readComments(file, commentsAfterSize);

    std::getline(file, maxValString);

    file.close();

    auto fileName = std::filesystem::path(getFilePath()).filename().string();
    auto fileSize = std::filesystem::file_size(getFilePath());

    fmt::println("File format: PPM\n");

    fmt::println(
            "The PBM format was invented by Jef Poskanzer in the 1980s as a format that allowed monochrome bitmaps to be transmitted within an email message as plain ASCII text, allowing it to survive any changes in text formatting.");
    fmt::println("Wiki: https://en.wikipedia.org/wiki/Netpbm\n");

    fmt::println("Image \"{}\" properties:\n", fileName);

    fmt::println("Height: {}", height);
    fmt::println("Width: {}", width);
    fmt::println("Size: {}B", fileSize);

    fmt::println("Magic number: {}", magicNumber);
    fmt::println("Max color value: {}", maxValString);

    /*
     * https://en.cppreference.com/w/cpp/filesystem/last_write_time
     */
    auto timeStamp = std::filesystem::last_write_time(getFilePath());
    // formatuje wedlug kodu z ww. strony
    fmt::println("{}", std::format("Last modified: {}\n", timeStamp));

}

auto PPM::containMessage() -> bool {
    if(!std::filesystem::exists(getFilePath())) {
        fmt::println("File does not exist");
        exit(1);
    }

    auto file = std::fstream(getFilePath(), std::ios::in | std::ios::binary);

    if(!file.good()){
        fmt::println("No permission to read the file");
        exit(1);
    }

    auto magicNumber = std::string();
    auto commentsBeforeSize = std::vector<std::string>();
    auto size = std::string();

    std::getline(file, magicNumber);
    readComments(file, commentsBeforeSize);

    std::getline(file, size);

    auto width = std::stoi(size.substr(0, size.find(' ')));
    auto height = std::stoi(size.substr(size.find(' ') + 1));

    auto possibleBits = width * height * 3;
    auto messageSize = getMessage().size() * 8 + 16;// Size of message + 2 size of 2 special characters

    return messageSize <= possibleBits;
}

auto PPM::readComments(std::fstream &file, std::vector<std::string> &comments) -> void {
    while (file.peek() == '#') {
        auto comment = std::string();
        std::getline(file, comment);
        comments.push_back(comment);
    }
}