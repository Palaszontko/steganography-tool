#include "CLI.hpp"
#include <fmt/core.h>
#include <string>
#include <fmt/color.h>
#include <filesystem>
#include <set>
#include "BMP.hpp"
#include "PPM.hpp"
#include "GIF.hpp"

auto supportedFormats = std::set<std::string>{".gif", ".bmp", ".ppm"};

auto info(std::string const &filePath) -> void {
    auto filesystemPath = std::filesystem::path(filePath);
    auto extension = filesystemPath.extension().string();

    if (supportedFormats.contains(extension)) {
        if (extension == ".gif") {
            auto gif = GIF(filePath);
            gif.getFileInfo();
        } else if (extension == ".bmp") {
            auto bmp = BMP(filePath);
            bmp.getFileInfo();
        } else if (extension == ".ppm") {
            auto ppm = PPM(filePath);
            ppm.getFileInfo();
        }
    } else {
        fmt::print(fmt::fg(fmt::color::red), "Unupported file format: {}\n", extension);
        fmt::println("Supported file formats: {}", fmt::join(supportedFormats, ", "));
    }
}

auto encrypt(std::string const &filePath, std::string const &message) -> void {
    auto filesystemPath = std::filesystem::path(filePath);
    auto extension = filesystemPath.extension().string();

    if (supportedFormats.contains(extension)) {
        if (extension == ".gif") {
            auto gif = GIF(filePath, message);
            gif.encodeMessageToFile();
        } else if (extension == ".bmp") {
            auto bmp = BMP(filePath, message);
            bmp.encodeMessageToFile();
        } else if (extension == ".ppm") {
            auto ppm = PPM(filePath, message);
            ppm.encodeMessageToFile();
        }
    } else {
        fmt::println("Unsupported file format: {}", extension);
        fmt::println("Supported file formats: {}", fmt::join(supportedFormats, ", "));
    }
}

auto decrypt(std::string const &filePath) -> void {
    auto filesystemPath = std::filesystem::path(filePath);
    auto extension = filesystemPath.extension().string();

    if (supportedFormats.contains(extension)) {
        if (extension == ".gif") {
            auto gif = GIF(filePath);
            gif.decodeMessageFromFile();
        } else if (extension == ".bmp") {
            auto bmp = BMP(filePath);
            bmp.decodeMessageFromFile();
        } else if (extension == ".ppm") {
            auto ppm = PPM(filePath);
            ppm.decodeMessageFromFile();
        }
    } else {
        fmt::println("Unsupported file format: {}", extension);
        fmt::println("Supported file formats: {}", fmt::join(supportedFormats, ", "));
    }
}

auto check(std::string const &filePath, std::string const &message) -> void {
    auto filesystemPath = std::filesystem::path(filePath);
    auto extension = filesystemPath.extension().string();

    auto containMessage = false;

    if (supportedFormats.contains(extension)) {
        if (extension == ".gif") {
            auto gif = GIF(filePath, message);
            containMessage = gif.containMessage();
        } else if (extension == ".bmp") {
            auto bmp = BMP(filePath, message);
            containMessage = bmp.containMessage();
        } else if (extension == ".ppm") {
            auto ppm = PPM(filePath, message);
            containMessage = ppm.containMessage();
        }

        fmt::println("The file {} contain the following message", containMessage ? "could" : "could not");

    } else {
        fmt::println("Unsupported file format: {}", extension);
        fmt::println("Supported file formats: {}", fmt::join(supportedFormats, ", "));
    }
}

auto printAsciiArtLine(std::string const &text) -> void {

    auto randomGreen = []() {
        auto random = std::rand() % 4;
        switch (random) {
            case 0:
                return fmt::color::green;
            case 1:
                return fmt::color::light_green;
            case 2:
                return fmt::color::sea_green;
            case 3:
                return fmt::color::spring_green;
            default:
                return fmt::color::white;
        };
    };

    for (auto c: text) {
        auto color = randomGreen();
        fmt::print(fmt::fg(color), "{}", c);
    }
}

auto help() -> void {
    fmt::print(fmt::fg(fmt::color::light_green), "CLI Steganography\n");
    fmt::print(fmt::emphasis::bold, "Supported file formats: {}\n", fmt::join(supportedFormats, ", "));
    fmt::print(fmt::emphasis::bold, "Flags:\n");
    fmt::println("-i, --info <file_path>                -> Display informations about the file");
    fmt::println("-e, --encrypt <file_path> <message>   -> Encrypt message to file");
    fmt::println("-d, --decrypt <file_path>             -> Dectypt message from file");
    fmt::println("-c, --check <file_path> <message>     -> Checks if file could contain specyfic message");
    fmt::println("-h, --help                            -> Display help\n");

    fmt::print(fmt::emphasis::bold, "Supported file formats properties:\n");
    fmt::print(fmt::emphasis::bold, "BMP:\n");
    fmt::println("Color depth: 24 bits and 32 bits");
    fmt::print(fmt::emphasis::bold, "PPM:\n");
    fmt::println("P3 and P6");
    fmt::print(fmt::emphasis::bold, "GIF:\n");
    fmt::println("GIF87a and GIF89a with global color table");
}


auto printAsciiArt() -> void {
    /*
     * https://patorjk.com/software/taag/#p=display&f=Doom&t=Steganography
     */
    printAsciiArtLine(" _____ _                                                     _           \n");
    printAsciiArtLine("/  ___| |                github.com/Palaszontko             | |          \n");
    printAsciiArtLine("\\ `--.| |_ ___  __ _  __ _ _ __   ___   __ _ _ __ __ _ _ __ | |__  _   _ \n");
    printAsciiArtLine(" `--. \\ __/ _ \\/ _` |/ _` | '_ \\ / _ \\ / _` | '__/ _` | '_ \\| '_ \\| | | |\n");
    printAsciiArtLine("/\\__/ / ||  __/ (_| | (_| | | | | (_) | (_| | | | (_| | |_) | | | | |_| |\n");
    printAsciiArtLine("\\____/ \\__\\___|\\__, |\\__,_|_| |_|\\___/ \\__, |_|  \\__,_| .__/|_| |_|\\__, |\n");
    printAsciiArtLine("                __/ |                   __/ |         | |           __/ |\n");
    printAsciiArtLine("               |___/                   |___/          |_|          |___/ \n\n");

}