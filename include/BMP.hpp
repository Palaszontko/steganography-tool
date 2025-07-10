#pragma once

#include <iostream>
#include <fmt/core.h>
#include <fstream>
#include <cmath>
#include <vector>
#include <filesystem>
#include <string>
#include <chrono>
#include "image.hpp"
#include "utils.hpp"

class BMP : public Image {

public:
    BMP(std::string const& file_path, std::string const& message = "") : Image(file_path, message) {};

    auto encodeMessageToFile() -> void override;
    auto decodeMessageFromFile() -> void override;
    auto getFileInfo() -> void override;
    auto containMessage() -> bool override;

private:
    /*
     * http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm
     * https://en.wikipedia.org/wiki/BMP_file_format
     */
#pragma pack(1)
    struct BMPHeader { //14
        uint16_t signature;
        uint32_t fileSize;
        uint32_t reserved;
        uint32_t dataOffset;
    } bmpHeader;

    struct DIBHeader { //40
        uint32_t headerSize;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bitsPerPixel;
        uint32_t compression;
        uint32_t imageSize;
        int32_t XpixelsPerM;
        int32_t YpixelsPerM;
        uint32_t ColorsUsed;
        uint32_t ColorsImportant;
    } dibHeader;
#pragma pack()
};