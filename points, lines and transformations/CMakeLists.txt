cmake_minimum_required(VERSION 3.26)
project(punkty__linie_i_przeksztalcenia)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/Cellar/sfml/2.6.1/include)
link_directories(/opt/homebrew/Cellar/sfml/2.6.1/lib)

add_executable(punkty__linie_i_przeksztalcenia zadanie2.cpp)

target_link_libraries(punkty__linie_i_przeksztalcenia sfml-graphics sfml-window sfml-system)