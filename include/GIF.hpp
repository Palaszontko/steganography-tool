#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <string>
#include <format>
#include <filesystem>
#include <chrono>
#include "utils.hpp"
#include "image.hpp"

class GIF : public Image {

public:
    GIF(std::string const& file_path, std::string const& message = "") : Image(file_path, message) {};

    auto encodeMessageToFile() -> void override;
    auto decodeMessageFromFile() -> void override;
    auto getFileInfo() -> void override;
    auto containMessage() -> bool override;
private:
    struct GIFHeaderAndLogicalScreenDescriptor {
        uint8_t signature[3];
        uint8_t version[3];
        uint16_t screenWidth;
        uint16_t screenHeight;
        uint8_t packed;
        uint8_t backgroundColor;
        uint8_t aspectRatio;
    };
};