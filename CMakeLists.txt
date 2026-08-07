cmake_minimum_required(VERSION 3.20)

project(forge
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(forge
    src/main.c
    src/token.c
)

target_include_directories(forge
    PRIVATE
        include
)