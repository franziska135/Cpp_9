#include "BitcoinExchange.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout   << "ERROR:\tProgram takes one parameter"
                    << std::endl;
        return 1;
    }
    try {
        BitcoinExchange Data;
        Data.loadFiles(argv[1]);

    } catch (std::exception &e) {
        std::cout << "Error:\t" << e.what() << std::endl;
        return 0;
    }
    return 1;
}