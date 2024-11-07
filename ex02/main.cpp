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
        std::cout << "Before:\t";
        list.printVector();

        std::clock_t start = std::clock();
        list.VecDivideRecursion();
        list.VecConquerRecursion();
        std::clock_t end = std::clock();
        
        std::cout << "After:\t";
        list.printVector();
        double duration = 1e6 * (end - start) / CLOCKS_PER_SEC;
        std::cout << std::fixed << std::setprecision(5);
        std::cout   << "Time to process a range of " << argc-1
                    << " elements with std::vector: " << duration << " us"
                    << std::endl;

        if (list.isSortedAscending())
            std::cout << "\nYES!" << std::endl;
        else
            std::cout << "\nBoo" << std::endl;
        
    } catch (std::exception &e) {
        std::cerr << "ERROR:\t" << e.what() << std::endl;
    }

}