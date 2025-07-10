#pragma once

#include <string>
#include <set>

class Image {

private:
    std::string filePath;
    std::string message;;

public:
    Image(std::string const& filePath, std::string const& message) : filePath(filePath), message(message) {};

    virtual auto encodeMessageToFile() -> void = 0;
    virtual auto decodeMessageFromFile() -> void = 0;
    virtual auto getFileInfo() -> void = 0;
    virtual auto containMessage() -> bool = 0;

    auto getFilePath() -> std::string{
        return filePath;
    }

    auto getMessage() -> std::string{
        return message;
    }
};
