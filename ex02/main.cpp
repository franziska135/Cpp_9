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

        /*sorting with std::vector container*/
        std::clock_t start = std::clock();
        list.VecMergeRecursion();
        list.VecInsertRecursion();
        std::clock_t end = std::clock();

        double duration = 1e6 * (end - start) / CLOCKS_PER_SEC;
        std::cout << std::fixed << std::setprecision(5);
        std::cout   << "Time to process a range of " << argc-1
                    << " elements with std::vector:\t" << duration << " us"
                    << std::endl;

        // /*sorting with std::deque container*/
        // start = std::clock();
        // list.DeqDivideRecursion();
        // list.DeqConquerRecursion();
        // end = std::clock();

        // duration = 1e6 * (end - start) / CLOCKS_PER_SEC;
        // std::cout << std::fixed << std::setprecision(5);
        // std::cout   << "Time to process a range of " << argc-1
        //             << " elements with std::deque:\t" << duration << " us"
        //             << std::endl;
        
        std::cout << "After:\t";
        list.printVector();

        std::cout << "\nTest if the input is sorted:\t";
        if (list.isSortedAscending())
            std::cout << "YES!" << std::endl;
        else
            std::cout << "Boo" << std::endl;
        
    } catch (std::exception &e) {
        std::cerr << "ERROR:\t" << e.what() << std::endl;
    }

}