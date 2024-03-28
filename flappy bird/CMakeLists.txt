cmake_minimum_required(VERSION 3.23)
project(Testowy)
include(FetchContent)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 14)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)
FetchContent_MakeAvailable(SFML)

add_executable(Testowy escape.cpp escape.h main.cpp)

target_link_libraries(Testowy sfml-graphics)
