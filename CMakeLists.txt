cmake_minimum_required(VERSION 3.31)

# Config
project(
        yac8
        LANGUAGES C
        VERSION 0.1.0
)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_SOURCE src/*.c)

include_directories(./include)
file(GLOB_RECURSE PROJECT_HEADERS include/*.h)

# Compilation
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_HEADERS})

# Dependencies
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

find_package(unofficial-sodium CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial-sodium::sodium)
