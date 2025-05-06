cmake_minimum_required(VERSION 3.28)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(yadro_test main.cpp
        app.cpp
        app.hpp)
