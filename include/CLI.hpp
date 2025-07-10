#pragma once

#include <iostream>
#include <string>

auto help() -> void;
auto info(std::string const& filePath) -> void;
auto encrypt(std::string const& filePath, std::string const& message) -> void;
auto decrypt(std::string const& filePath) -> void;
auto check(std::string const& filePath, std::string const& message) -> void;



auto printAsciiArtLine(std::string const& text) -> void;
auto printAsciiArt() -> void;
