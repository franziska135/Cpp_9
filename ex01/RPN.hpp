#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <stack>

class RPN {
    private:
    public:
        RPN     (void);
        RPN     (const RPN& other);
        ~RPN    ();

        RPN&    operator=(const RPN &other);

        void    calculate(std::string input);
};

#endif