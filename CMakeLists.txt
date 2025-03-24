cmake_minimum_required(VERSION 3.31)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(REPO_URL "https://github.com/v-maxson/yac8")

# Config
project(
        yac8
        LANGUAGES C
        VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_SOURCE src/*.c)

include_directories(./include)
file(GLOB_RECURSE PROJECT_HEADERS include/*.h)

# Generate version.h
set(GENERATED_HEADERS_DIR ${CMAKE_BINARY_DIR}/yac_generated)
configure_file(include/yac_version.h.in ${GENERATED_HEADERS_DIR}/yac_version.h)
include_directories(${GENERATED_HEADERS_DIR})

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
