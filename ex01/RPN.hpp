#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <sstream>
#include <stack>
#include <algorithm>
#include <climits>


class RPN {
    private:
    public:
        RPN     (void);
        RPN     (const RPN& other);
        ~RPN    ();

        RPN&    operator=(const RPN &other);

        void    analyze(std::string input);
        int     calculate(int a, int b, std::string operation);
        int     isInRangeStr(std::string number);

        class   insufficientOperators : public std::exception {
            public:
                const char * what() const throw();
        };

        class   insufficientOperands : public std::exception {
            public:
                const char * what() const throw();
        };

        class   zeroDivision : public std::exception {
            public:
                const char * what() const throw();
        };

        class   invalidToken : public std::exception {
            public:
                const char * what() const throw();
        };

        class   Overflow : public std::exception {
            public:
                const char * what() const throw();
        };
};

#endif