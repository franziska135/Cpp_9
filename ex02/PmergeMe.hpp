#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <sstream>

class PmergeMe {
    private:
        std::vector<int>                _VecArray;
        std::vector<std::vector<int> >   _VecMatrix;
    public:
        PmergeMe        (void);
        PmergeMe        (const PmergeMe& other);
        ~PmergeMe       ();

        PmergeMe&       operator=(const PmergeMe &other);

        void    parseInput (int argc, char *argv[]);
        void    isInRangeStr(std::string number);
        void    printVector(std::vector<int> v);
        void    printMatrix();
        void    downRecursionVec();
        void    separateInsert(int index);

        class   NumericArg : public std::exception {
            public:
                const char * what() const throw();
        };

        class   Overflow : public std::exception {
            public:
                const char * what() const throw();
        };
};

#endif