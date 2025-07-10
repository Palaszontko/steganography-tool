#include "fmt/core.h"
#include "fmt/ranges.h"
#include <vector>
#include <string>


auto messageToBinary(std::string const &message) -> std::vector<int> {
    auto messageBinary = std::vector<int>();
    messageBinary.insert(messageBinary.begin(), {1, 0, 0, 1, 1, 1, 0, 1});

    for (auto letter: message) {
        for (auto bit: fmt::format("{:08b}", letter)) { //
            messageBinary.push_back(bit - '0');
        }
    }

    messageBinary.insert(messageBinary.end(), {1,0,0,0,1,1,0,1});

    return messageBinary;
}

auto readDecodedMessage(std::vector<int> const &messageArray) -> void {
    auto binaryLetters = std::vector<char>();

    for (int i = 0; i < messageArray.size(); i += 8) {
        auto letterValue = 0;
        for (int j = 0; j < 8; ++j) {
            letterValue = (letterValue << 1) | messageArray[i + j];
        }

        if(i == 0 && letterValue != 0x9D){ //if false it means that my steganography algorithm didnt encode any message inside this file bcs of special sign
            fmt::println("{}", "File doesn't contain message");
            return;
        }

        if (letterValue == 0x8D) { //end sign
            break;
        } else if(letterValue == 0x9D) { //start sign
            continue;
        } else {
            binaryLetters.push_back((char) letterValue);
        }
    }

    fmt::println("Message decoded successfully:\n{}", fmt::join(binaryLetters, ""));

}

auto getLSB(uint8_t const &value) -> int {
    return value & 1;
}