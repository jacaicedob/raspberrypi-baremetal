cmake_minimum_required(VERSION 3.16)
project(rpi_bm C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(boot)
add_subdirectory(examples)