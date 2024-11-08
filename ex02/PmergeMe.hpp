#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

#define FIRSTUP 1
#define SECONDUP 0

class PmergeMe {
    private:
        std::vector<int>                _VecArray;
        std::deque<int>                 _DeqArray;

        std::vector<std::vector<int> >  _VecMatrix;
        std::deque<std::deque<int> >    _DeqMatrix;
        std::vector<int>                _moves;
        std::vector<int>                _indexInsert;
    public:
        PmergeMe        (void);
        PmergeMe        (const PmergeMe& other);
        ~PmergeMe       ();

        PmergeMe&       operator=(const PmergeMe &other);

        void    parseInput (int argc, char *argv[]);
        void    isInRangeStr(std::string number);
        void    hasDoublicates(std::vector<int> &vec);
        void    printVector();
        void    printMatrix();
        void    VecMergeRecursion();
        void    VecMerge(int index);
        void    VecApplyMovesMerge(int index);
        void    VecInsertRecursion();
        void    VecInsert();
        void    VecApplyMovesInsert(int index);
        
        // void    DeqDivideRecursion();
        // void    DeqDivide(int index);
        // void    DeqApplyMovesDivide(int index);
        // void    DeqConquerRecursion();
        // void    DeqConquerMerge();
        // void    DeqApplyMovesConquer(int index);
        bool    isSortedAscending();

        class   NumericArg : public std::exception {
            public:
                const char * what() const throw();
        };

        class   noDoubles : public std::exception {
            public:
                const char * what() const throw();
        };

        class   Overflow : public std::exception {
            public:
                const char * what() const throw();
        };
};

#endif