#include "PmergeMe.hpp"

/*take out*/


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Error:\t Insufficient amount of arguments: ./a.out <argn> <argn+1> ..." << std::endl;
        return 0;
    }

    try {
        PmergeMe list;
        list.parseInput(argc, argv);
        // std::cout << "\nvector b4 finishing divide:\n";
       
       list.printMatrix();

        list.DivideRecursion();
        
        // std::cout << "\nvector after finishing divide conquer starts:\n";
        list.printMatrix();

        list.ConquerRecursion();
        std::cout << "GRAND FINAL:" << std::endl;
        if (list.isSortedAscending())
            std::cout << "YES!" << std::endl;
        else
            std::cout << "try again" << std::endl;

        // list.printMatrix();
        //list.printVector(0);
        
    } catch (std::exception &e) {
        std::cerr << "ERROR:\t" << e.what() << std::endl;
    }

}