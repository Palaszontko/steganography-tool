#include <fmt/core.h>
#include <fmt/color.h>
#include "CLI.hpp"

auto main(int argc, char *argv[]) -> int {
    printAsciiArt();

    if (argc == 1) {
        help();
        return 0;
    }

    auto flag = std::string(argv[1]);

    if (flag == "-i" || flag == "--info" ) {
        if (argc < 3) {
            fmt::print(fmt::fg(fmt::color::red), "No file path provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if (argc > 3) {
            fmt::print(fmt::fg(fmt::color::red), "Too many arguments\n");
            fmt::println("For help use -h or --help");
            return 0;
        }
        info(argv[2]);
    } else if (flag == "-e" || flag == "--encrypt") {
        if (argc == 2) {
            fmt::print(fmt::fg(fmt::color::red), "No file path provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if (argc == 3) {
            fmt::print(fmt::fg(fmt::color::red), "No message provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if (argc > 4) {
            fmt::print(fmt::fg(fmt::color::red), "Too many arguments\n");
            fmt::println("For help use -h or --help");
            return 0;
        }
        encrypt(argv[2], argv[3]);
    } else if (flag == "-d" || flag == "--decrypt") {
        if (argc < 3) {
            fmt::print(fmt::fg(fmt::color::red), "No file path provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if (argc > 3) {
            fmt::print(fmt::fg(fmt::color::red), "Too many arguments\n");
            fmt::println("For help use -h or --help");
            return 0;
        }
        decrypt(argv[2]);
    } else if (flag == "-c" || flag == "--check") {
        if (argc < 3) {
            fmt::print(fmt::fg(fmt::color::red), "No file path provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if(argc < 4) {
            fmt::print(fmt::fg(fmt::color::red), "No message provided\n");
            fmt::println("For help use -h or --help");
            return 0;
        } else if (argc > 4) {
            fmt::print(fmt::fg(fmt::color::red), "Too many arguments\n");
            fmt::println("For help use -h or --help");
            return 0;
        }
        check(argv[2], argv[3]);
    } else if (flag == "-h" || flag == "--help") {
        if (argc > 2) {
            fmt::print(fmt::fg(fmt::color::red), "Too many arguments\n");
            fmt::println("For help use -h or --help");
            return 0;
        }
        help();
    } else {
        fmt::print(fmt::fg(fmt::color::red), "Unknown flag: {}\n", flag);
        fmt::println("For help use -h or --help");
    }

    return 0;
}