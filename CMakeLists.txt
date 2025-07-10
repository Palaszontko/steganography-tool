cmake_minimum_required(VERSION 3.27)
project(Steganography)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

include(FetchContent)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

# Source files
set(SOURCES
    src/main.cpp
    src/image.cpp
    src/BMP.cpp
    src/utils.cpp
    src/PPM.cpp
    src/GIF.cpp
    src/CLI.cpp
)

# Header files
set(HEADERS
    include/image.hpp
    include/BMP.hpp
    include/utils.hpp
    include/PPM.hpp
    include/GIF.hpp
    include/CLI.hpp
)

add_executable(steganography ${SOURCES} ${HEADERS})

target_link_libraries(steganography fmt)
