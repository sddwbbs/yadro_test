cmake_minimum_required(VERSION 3.28)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

add_executable(yadro_test
        main.cpp
        app.cpp
        app.hpp)

enable_testing()

add_executable(unit_tests
        tests/test_input.cpp
        app.cpp
        app.hpp)

target_link_libraries(unit_tests gtest gtest_main pthread)

add_test(NAME AllTests COMMAND unit_tests)