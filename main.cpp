#include "app.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Необходимо задать файл с входными данными!" << std::endl;
        return 1;
    }

    try {
        App::start(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}