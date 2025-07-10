#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <fmt/core.h>
#include <string>
#include "utils.hpp"
#include "image.hpp"
#include <filesystem>
#include <chrono>

class PPM : public Image {
public:
    PPM(std::string const& file_path, std::string const& message = "") : Image(file_path, message) {};

    auto encodeMessageToFile() -> void override;
    auto decodeMessageFromFile() -> void override;
    auto getFileInfo() -> void override;
    auto containMessage() -> bool override;

    auto readComments(std::fstream &file, std::vector<std::string> &comments) -> void;

};
