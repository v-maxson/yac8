cmake_minimum_required(VERSION 3.31)

# Config
project(
        yac8
        LANGUAGES C
        VERSION 0.2.0
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCE src/*.c)

include_directories(./include)
file(GLOB_RECURSE PROJECT_HEADERS include/*.h)

# Compilation
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_HEADERS})

# Dependencies
set(SDL_SHARED OFF)
add_subdirectory(deps/SDL)
include_directories(deps/SDL/include)
target_link_libraries(${PROJECT_NAME} SDL2-static)

set(SODIUM_DISABLE_TESTS ON)
set(SODIUM_MINIMAL ON)
add_subdirectory(deps/sodium)
target_link_libraries(${PROJECT_NAME} sodium)
