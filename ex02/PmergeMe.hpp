#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

#define FIRSTUP 1
#define SECONDUP 0

class PmergeMe {
    private:
        std::vector<int>                _VecArray;
        std::vector<std::vector<int> >   _VecMatrix;
        std::vector<int>                _moves;
        std::vector<int>                _indexInsert;
    public:
        PmergeMe        (void);
        PmergeMe        (const PmergeMe& other);
        ~PmergeMe       ();

        PmergeMe&       operator=(const PmergeMe &other);

        void    parseInput (int argc, char *argv[]);
        void    isInRangeStr(std::string number);
        void    printVector();
        void    printMatrix();
        void    DivideRecursion();
        void    separateDivide(int index);
        void    applyMovesDivide(int index);
        void    ConquerRecursion();
        void    ConquerMerge();
        void    applyMovesConquer(int index);
        bool    isSortedAscending();

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