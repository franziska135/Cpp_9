#include "RPN.hpp"

RPN::RPN() {
}

RPN::RPN(const RPN& other) {
    *this = other;
}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {}
    return *this;
}

RPN::~RPN  () {
}

const char* RPN::invalidToken::what() const throw () {
    return "Argument contains invalid token";
}

const char* RPN::insufficientOperands::what() const throw () {
    return "Insufficient operands to perform calculation";
}

const char* RPN::zeroDivision::what() const throw () {
    return "Division by zero";
}

const char* RPN::insufficientOperators::what() const throw () {
    return "Insufficient operators";
}

const char* RPN::Overflow::what() const throw () {
    return "Out of range";
}

int RPN::calculate(int a, int b, std::string operation) {
    if (operation == "+") {
        if ((b < 0 && a < INT_MIN - b) || (b > 0 && a > INT_MAX - b))
            throw Overflow();
        return (a + b);
    }
    else if (operation == "-") {
        if ((b > 0 && a < INT_MIN + b) || (b < 0 && a > INT_MAX + b))
            throw Overflow();
        return (a - b);
    }
    else if (operation == "*") {
        if ((b > 0 && (a < INT_MIN / b || a > INT_MAX / b))
            || (b < 0 && (a > INT_MIN / b || a < INT_MAX / b )))
            throw Overflow();
        return (a * b);
    }
    else if (operation == "/") {
        if (b == 0)
            throw zeroDivision();
        if (b == -1 && a == INT_MIN)
            throw Overflow();
        return (a / b);
    }
    return -1;
}

int RPN::isInRangeStr(std::string number) {
    if ((number.size() >= 10 && number > "2147483647")
        || number.size() > 10)
        return false;
    return true;
}

void    RPN::analyze(std::string line) {
    std::stack<int>     stack;
    
    std::istringstream  argument(line);
    std::string         token;

    while (argument >> token) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (stack.size() < 2)
                throw insufficientOperands();
            else {
                int b = stack.top();
                stack.pop();
                int a = stack.top();
                stack.pop();
                
                int result = this->calculate(a, b, token);
                stack.push(result);
            }
        }
        else { //addition to subject: evaluating non-single digit numbers
            size_t i = 0;

            while (i < token.size()) {
                if (!isdigit(token[i]))
                    break;
                i++;
            } 
            
            if (i != token.size())
                throw invalidToken();
            if (!isInRangeStr(token))
                throw Overflow();

            int value;
            std::istringstream(token) >> value;
            stack.push(value);
        }
    }
    if (stack.size() == 1)
        std::cout << stack.top() << std::endl;
    else
        throw insufficientOperators();
}