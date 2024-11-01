#include "PmergeMe.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error:\t Insufficient amount of arguments: ./a.out <argn> <argn+1> ..." << std::endl;
        return 0;
    }

    try {
        PmergeMe list;
        list.parseInput(argc, argv);

        list.downRecursionVec();
            std::cout << "\nvector after finish:\n";
    list.printMatrix();
        
    } catch (std::exception &e) {
        std::cerr << "ERROR:\t" << e.what() << std::endl;
    }

}