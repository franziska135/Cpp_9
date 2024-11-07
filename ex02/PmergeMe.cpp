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

const char* PmergeMe::noDoubles::what() const throw () {
    return "Arguments shall not contain doublicates";
}

const char* PmergeMe::Overflow::what() const throw () {
    return "Out of range";
}

void    PmergeMe::printVector() {

    if (_VecMatrix[0].empty()) {
        std::cout << "Vector is empty" << std::endl;
        return ;
    }

    size_t i = 0;

    while (i < 5 && i < _VecMatrix[0].size()) {
        std::cout << _VecMatrix[0][i] << " ";
        i++;
    }
    
    if (_VecMatrix[0].size() > 5)
        std::cout << " [...]";
    std::cout << std::endl;
}

void PmergeMe::isInRangeStr(std::string number) {
    if ((number.size() >= 10 && number > "2147483647")
        || number.size() > 10)
        throw Overflow();
}

void    PmergeMe::hasDoublicates(std::vector<int> &v) {
    std::vector<int> copy = v;
    
    std::sort(copy.begin(), copy.end());
    for (size_t i = 0; i < copy.size() - 1; i++) {
        if (copy[i] == copy[i + 1])
            throw noDoubles();
    }
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
    try {
        hasDoublicates(_VecArray);
    } catch (std::exception &e) {
        throw ;
    }
    _VecMatrix[0] = _VecArray;
}

void    PmergeMe::separateDivide(int index) {
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

void    PmergeMe::applyMovesDivide(int index) {
    std::vector <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (i / 2 >= _moves.size())
            smaller.push_back(_VecMatrix[index][i]);
        else  {
            if (_moves[i / 2] == FIRSTUP) {
                bigger.push_back(_VecMatrix[index][i]);
                smaller.push_back(_VecMatrix[index][i + 1]);
            } else {
                bigger.push_back(_VecMatrix[index][i + 1]);
                smaller.push_back(_VecMatrix[index][i]);
            }
            if (!_VecMatrix[1].empty() && (_VecMatrix[1].size() % 2 != 0 || (_VecMatrix[1].size() + _VecMatrix[0].size()) % 2 != 0)
            && i + 3 == _VecMatrix[index].size())
                smaller.push_back(_VecMatrix[index][i + 2]);
        }
    }

    if (!_VecMatrix[index].empty())
        _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
}

void    PmergeMe::DivideRecursion() {
    if (_VecMatrix[0].size() == 2 && _VecMatrix[0][0] && _VecMatrix[0][1]
        && _VecMatrix[0][0] > _VecMatrix[0][1]) {
            for (int i = _VecMatrix.size() - 1; i >= 0; i--)
                std::swap(_VecMatrix[i][0], _VecMatrix[i][1]);
        }
    if (_VecMatrix[0].size() <= 2)
        return ;

    if (_VecMatrix.size() < 2)
        separateDivide(0);
    else {
        separateDivide(0);
        for (int i = _VecMatrix.size() - 1; i > 1; i--)
            applyMovesDivide(i);
    }

    if (!_moves.empty())
        _moves.clear();
    DivideRecursion();
}


void    PmergeMe::ConquerMerge() {

    std::vector<int>::iterator pos;
    
    _VecMatrix[0].insert(_VecMatrix[0].begin(), _VecMatrix[1][0]);
    _VecMatrix[1][0] = -1;
    _VecMatrix[1].insert(_VecMatrix[1].begin(), -1);
    _indexInsert.push_back(0);
    
    for (size_t i = 0; i < _VecMatrix[1].size(); i++) {
        if (_VecMatrix[1][i] != -1) {
            int valueInsert = _VecMatrix[1][i];
            
            /*insert with bin sort bc jakobs tal does not care for now*/
            pos = std::lower_bound(_VecMatrix[0].begin(), _VecMatrix[0].end(), valueInsert);
            int targetIndex = std::distance(_VecMatrix[0].begin(), pos);
            
            _indexInsert.push_back(targetIndex);
            _VecMatrix[0].insert(pos, valueInsert);
            _VecMatrix[1][i] = -1;
            _VecMatrix[1].insert(_VecMatrix[1].begin(), -1);
        }
    }
    _VecMatrix.erase(_VecMatrix.begin() + 1);
}

void    PmergeMe::applyMovesConquer(int index) {
    for (size_t j = 0; j < _VecMatrix[index + 1].size(); j++) {
        int     valueInsert = _VecMatrix[index + 1][j];
        
        if (j >= _indexInsert.size())
            _VecMatrix[index].push_back(valueInsert);
        else {
            size_t  targetIndex = _indexInsert[j];
            std::vector<int>::iterator pos = _VecMatrix[index].begin() + targetIndex;
            _VecMatrix[index].insert(pos, valueInsert);
        }
    }
    _VecMatrix.erase(_VecMatrix.begin() + index + 1);
}

void    PmergeMe::ConquerRecursion() {
    if (_VecMatrix.size() == 1)
        return ;
    ConquerMerge();

    for (int i = _VecMatrix.size() - 2; i > 0; i -= 2)
        applyMovesConquer(i);

    if (!_indexInsert.empty())
        _indexInsert.clear();
    ConquerRecursion();
}

void PmergeMe::printMatrix() {
    for (size_t i = 0; i < _VecMatrix.size() / 16; i++) {
        for (size_t j = 0; j < _VecMatrix[i].size(); j++)
            std::cout << "\t" << _VecMatrix[i][j] << " ";
        std::cout << std::endl;
    } std::cout << std::endl;
}

bool PmergeMe::isSortedAscending() {
    std::vector<int> vec = _VecMatrix[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            std::cout << "index: " << i << " value: " << vec[i] << std::endl;
            std::cout << "value before : " << vec[i - 1] << std::endl;
            std::cout << "value after: " << vec[i + 1] << std::endl;
            std::cout << "value 2. after: " << vec[i + 2] << std::endl;
            
            return false;
        }
    }
    return true;
}