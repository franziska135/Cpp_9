#include "PmergeMe.hpp"

PmergeMe::PmergeMe() { }

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _VecArray = other._VecArray;
        _moves = other._moves;
        _VecMatrix = other._VecMatrix;
        /*fill this*/
    } return *this;
}

PmergeMe::~PmergeMe  () {
}

const char* PmergeMe::NumericArg::what() const throw () {
    return "Arguments must be numeric and positive";
}

const char* PmergeMe::Overflow::what() const throw () {
    return "Out of range";
}

void    PmergeMe::printVector(std::vector<int> v) {
    if (v.empty()) {
        std::cout << "Vector is empty" << std::endl;
        return ;
    }

    int limit = v.size(), i = 0;
    
    if (v.size() > 5)
        limit = 5;
    while (i < limit - 1) {
        std::cout << v[i] << " ";
        i++;
    } std::cout << v[v.size() - 1];
    
    if (v.size() > 5)
        std::cout << " [...]";
    std::cout << std::endl;
}

void PmergeMe::isInRangeStr(std::string number) {
    if ((number.size() >= 10 && number > "2147483647")
        || number.size() > 10)
        throw Overflow();
}

void    PmergeMe::parseInput(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string numberStr = argv[i];
        int number;

        try {
            size_t i = 0;
            while (i < numberStr.size()) {
                if (!isdigit(numberStr[i]))
                    break;
                i++;
            } if (i != numberStr.size())
                throw NumericArg();

            isInRangeStr(numberStr);

            std::istringstream iss(numberStr);
            if (!(iss >> number) || number <= 0)
                throw NumericArg();
            std::string remnant;
            if (iss >> remnant)
                throw NumericArg();
        } catch (std::exception &e) {
            throw ;
        } _VecArray.push_back(number);
    } if (_VecMatrix.size() < 1)
        _VecMatrix.resize(1);
    _VecMatrix[0] = _VecArray;
}

void    PmergeMe::separateInsert(int index) {
    std::vector <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (_VecMatrix[index][i] > _VecMatrix[index][i + 1]) {
            bigger.push_back(_VecMatrix[index][i]);
            smaller.push_back(_VecMatrix[index][i + 1]);
            _moves.push_back(FIRSTUP);
        } else {
            bigger.push_back(_VecMatrix[index][i + 1]);
            smaller.push_back(_VecMatrix[index][i]);
            _moves.push_back(SECONDUP);
        }
    } if (_VecMatrix[index].size() % 2 != 0)
        smaller.push_back(_VecMatrix[index].back());

    if (!_VecMatrix[index].empty())
    _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
}

void    PmergeMe::applyMovesInsert(int index) {
    std::vector <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (_moves[i / 2] == FIRSTUP) {
            bigger.push_back(_VecMatrix[index][i]);
            smaller.push_back(_VecMatrix[index][i + 1]);
        } else {
            bigger.push_back(_VecMatrix[index][i + 1]);
            smaller.push_back(_VecMatrix[index][i]);
        }
    } if (_VecMatrix[index].size() % 2 != 0)
        smaller.push_back(_VecMatrix[index].back());
    if (!_VecMatrix[index].empty())
        _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
}

void    PmergeMe::DivideRecursion() {

    if (_VecMatrix[0].size() <= 2)
        return ;
    
    separateInsert(0);
    for (int i = _VecMatrix.size() - 1; i > 1; i--)
        applyMovesInsert(i);
    
    if (!_moves.empty())
        _moves.clear();

    DivideRecursion();
}

void    PmergeMe::ConquerRecursion() { }

void PmergeMe::printMatrix() {
    for (size_t i = 0; i < _VecMatrix.size(); i++) {
        for (size_t j = 0; j < _VecMatrix[i].size(); j++)
            std::cout << _VecMatrix[i][j] << " ";
        std::cout << std::endl;
    } std::cout << std::endl;
}