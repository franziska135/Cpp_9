#include "RPN.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Error:\t Invalid amount of arguments: ./a.out <arg>" << std::endl;
        return 1;
    }
    try {
        RPN calculator;
        //int result;
        
        //calculate result
        calculator.analyze(argv[1]);
        //std::cout << result << std::endl;
        return 1;
    }
    catch (std::exception &e) {
        std::cout << "ERROR:\t" << e.what() << std::endl;
        return 0;
    }
    return 1;
}