#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <sstream>

#include "../app.hpp"

using std::string;

string load_file(const string& path) {
    std::ifstream file(path);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

TEST(AppTest, Handle_simple_input_file) {
    const char* input_path = "../tests/test_case_1/input.txt";

    testing::internal::CaptureStdout();
    App::start(input_path);
    string output = testing::internal::GetCapturedStdout();
    string expected_output = load_file("../tests/test_case_1/expected.txt");

    EXPECT_EQ(output, expected_output);
}

TEST(AppTest, Handle_file_not_specified) {
    string output;
    string expected_output = "Ошибка при открытии файла";

    try {
        App::start("trash.txt");
    } catch (std::exception& e) {
        output = e.what();
    }

    EXPECT_EQ(output, expected_output);
}

TEST(AppTest, Handle_file_format_incorrect_1) {
    string output;
    string expected_output = "Неверные входные данные в строке: 09:00 19:00 trash";

    const char* input_path = "../tests/test_case_3/input.txt";

    try {
        App::start(input_path);
    } catch (std::exception& e) {
        output = e.what();
    }

    EXPECT_EQ(output, expected_output);
}

TEST(AppTest, Handle_file_format_incorrect_2) {
    string output;
    string expected_output = "Неверные входные данные в строке: 08:48 1 client1 trash";

    const char* input_path = "../tests/test_case_4/input.txt";

    try {
        App::start(input_path);
    } catch (std::exception& e) {
        output = e.what();
    }

    EXPECT_EQ(output, expected_output);
}

TEST(AppTest, Handle_file_format_incorrect_3) {
    string output;
    string expected_output = "Неверные входные данные в строке: 09:54 2 client1 1 trash";

    const char* input_path = "../tests/test_case_5/input.txt";

    try {
        App::start(input_path);
    } catch (std::exception& e) {
        output = e.what();
    }

    EXPECT_EQ(output, expected_output);
}