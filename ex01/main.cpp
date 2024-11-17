#include "RPN.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Error:\tInvalid amount of arguments: ./a.out <arg>" << std::endl;
        return 0;
    }
    try {
        RPN calculator;
        calculator.analyze(argv[1]);
        return 1;
    }
    catch (std::exception &e) {
        std::cerr << "ERROR:\t" << e.what() << std::endl;
        return 0;
    }
    return 1;
}