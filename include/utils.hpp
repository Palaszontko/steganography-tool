#pragma once

#include <string>
#include <vector>
#include <cstdint>

auto messageToBinary(std::string const& message) -> std::vector<int>;
auto readDecodedMessage(std::vector<int> const& messageArray) -> void;
auto getLSB(uint8_t const& value) -> int;
