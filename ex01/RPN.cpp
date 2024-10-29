#include "RPN.hpp"

RPN::RPN() {
}

RPN::RPN(const RPN& other) {
    *this = other;
}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
    }
    return *this;
}

RPN::~RPN  () {
}

void    RPN::calculate(std::string line) {
    std::stack<int>  stack;
    
}