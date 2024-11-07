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

/*----------------------------------*/
/*---------Print functions----------*/
/*----------------------------------*/

void PmergeMe::printMatrix() {
    for (size_t i = 0; i < _VecMatrix.size() / 16; i++) {
        for (size_t j = 0; j < _VecMatrix[i].size(); j++)
            std::cout << "\t" << _VecMatrix[i][j] << " ";
        std::cout << std::endl;
    } std::cout << std::endl;
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

/*-----------------------------------*/
/*--helper function - is it sortet?--*/
/*-----------------------------------*/

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

/*----------------------------------*/
/*Error management and input parsing*/
/*----------------------------------*/

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
        } 
        _VecArray.push_back(number);
        _DeqArray.push_back(number);
    } if (_VecMatrix.size() < 1)
        _VecMatrix.resize(1);
    if (_DeqMatrix.size() < 1)
        _DeqMatrix.resize(1);
    try {
        hasDoublicates(_VecArray);
    } catch (std::exception &e) {
        throw ;
    }
    _VecMatrix[0] = _VecArray;
    _DeqMatrix[0] = _DeqArray;
}

/*-----------------------------------*/
/*Ford Johnson: with vector container*/
/*-----------------------------------*/

/*first step: going down to rock bottom*/

void    PmergeMe::VecDivideRecursion() {
    if (_VecMatrix[0].size() == 2 && _VecMatrix[0][0] && _VecMatrix[0][1]
        && _VecMatrix[0][0] > _VecMatrix[0][1]) {
            for (int i = _VecMatrix.size() - 1; i >= 0; i--)
                std::swap(_VecMatrix[i][0], _VecMatrix[i][1]);
        }
    if (_VecMatrix[0].size() <= 2)
        return ;

    if (_VecMatrix.size() < 2)
        VecDivide(0);
    else {
        VecDivide(0);
        for (int i = _VecMatrix.size() - 1; i > 1; i--)
            VecApplyMovesDivide(i);
    }

    if (!_moves.empty())
        _moves.clear();
    VecDivideRecursion();
}

/*second step: going up from rock bottom*/

void    PmergeMe::VecConquerRecursion() {
    if (_VecMatrix.size() == 1)
        return ;
    VecConquerMerge();

    for (int i = _VecMatrix.size() - 2; i > 0; i -= 2)
        VecApplyMovesConquer(i);

    if (!_indexInsert.empty())
        _indexInsert.clear();
    VecConquerRecursion();
}

/*implementations*/

void    PmergeMe::VecDivide(int index) {
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

void    PmergeMe::VecApplyMovesDivide(int index) {
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

void    PmergeMe::VecConquerMerge() {

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

void    PmergeMe::VecApplyMovesConquer(int index) {
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

// /*-----------------------------------*/
// /*Ford Johnson: with deque container-*/
// /*-----------------------------------*/

// /*first step: going down to rock bottom*/

// void    PmergeMe::DeqDivideRecursion() {
//     if (_DeqMatrix[0].size() == 2 && _DeqMatrix[0][0] && _DeqMatrix[0][1]
//         && _DeqMatrix[0][0] > _DeqMatrix[0][1]) {
//             for (int i = _DeqMatrix.size() - 1; i >= 0; i--)
//                 std::swap(_DeqMatrix[i][0], _DeqMatrix[i][1]);
//         }
//     if (_DeqMatrix[0].size() <= 2)
//         return ;

//     if (_DeqMatrix.size() < 2)
//         DeqDivide(0);
//     else {
//         DeqDivide(0);
//         for (int i = _DeqMatrix.size() - 1; i > 1; i--)
//             DeqApplyMovesDivide(i);
//     }

//     if (!_moves.empty())
//         _moves.clear();
//     DeqDivideRecursion();
// }

// /*second step: going up from rock bottom*/

// void    PmergeMe::DeqConquerRecursion() {
//     if (_DeqMatrix.size() == 1)
//         return ;
//     DeqConquerMerge();

//     for (int i = _DeqMatrix.size() - 2; i > 0; i -= 2)
//         DeqApplyMovesConquer(i);

//     if (!_indexInsert.empty())
//         _indexInsert.clear();
//     DeqConquerRecursion();
// }

// /*implementations*/

// void    PmergeMe::DeqDivide(int index) {
//     std::deque <int> bigger, smaller;

//     for (size_t i = 0; i + 1 < _DeqMatrix[index].size(); i += 2) {
//         if (_DeqMatrix[index][i] > _DeqMatrix[index][i + 1]) {
//             bigger.push_back(_DeqMatrix[index][i]);
//             smaller.push_back(_DeqMatrix[index][i + 1]);
//             _moves.push_back(FIRSTUP);
//         } else {
//             bigger.push_back(_DeqMatrix[index][i + 1]);
//             smaller.push_back(_DeqMatrix[index][i]);
//             _moves.push_back(SECONDUP);
//         }
//     } if (_DeqMatrix[index].size() % 2 != 0)
//         smaller.push_back(_DeqMatrix[index].back());

//     if (!_DeqMatrix[index].empty())
//     _DeqMatrix.erase(_DeqMatrix.begin() + index);
//     _DeqMatrix.insert(_DeqMatrix.begin() + index, smaller);
//     _DeqMatrix.insert(_DeqMatrix.begin() + index, bigger);
// }

// void    PmergeMe::DeqApplyMovesDivide(int index) {
//     std::deque <int> bigger, smaller;

//     for (size_t i = 0; i + 1 < _DeqMatrix[index].size(); i += 2) {
//         if (i / 2 >= _moves.size())
//             smaller.push_back(_DeqMatrix[index][i]);
//         else  {
//             if (_moves[i / 2] == FIRSTUP) {
//                 bigger.push_back(_DeqMatrix[index][i]);
//                 smaller.push_back(_DeqMatrix[index][i + 1]);
//             } else {
//                 bigger.push_back(_DeqMatrix[index][i + 1]);
//                 smaller.push_back(_DeqMatrix[index][i]);
//             }
//             if (!_DeqMatrix[1].empty() && (_DeqMatrix[1].size() % 2 != 0 || (_VecMatrix[1].size() + _VecMatrix[0].size()) % 2 != 0)
//             && i + 3 == _DeqMatrix[index].size())
//                 smaller.push_back(_DeqMatrix[index][i + 2]);
//         }
//     }

//     if (!_DeqMatrix[index].empty())
//         _DeqMatrix.erase(_DeqMatrix.begin() + index);
//     _DeqMatrix.insert(_DeqMatrix.begin() + index, smaller);
//     _DeqMatrix.insert(_DeqMatrix.begin() + index, bigger);
// }

// void    PmergeMe::DeqConquerMerge() {

//     std::vector<int>::iterator pos;
    
//     _DeqMatrix[0].insert(_DeqMatrix[0].begin(), _DeqMatrix[1][0]);
//     _DeqMatrix[1][0] = -1;
//     _DeqMatrix[1].insert(_DeqMatrix[1].begin(), -1);
//     _indexInsert.push_back(0);
    
//     for (size_t i = 0; i < _DeqMatrix[1].size(); i++) {
//         if (_DeqMatrix[1][i] != -1) {
//             int valueInsert = _DeqMatrix[1][i];
            
//             /*insert with bin sort bc jakobs tal does not care for now*/
//             pos = std::lower_bound(_DeqMatrix[0].begin(), _DeqMatrix[0].end(), valueInsert);
//             int targetIndex = std::distance(_DeqMatrix[0].begin(), pos);
            
//             _indexInsert.push_back(targetIndex);
//             _DeqMatrix[0].insert(pos, valueInsert);
//             _DeqMatrix[1][i] = -1;
//             _DeqMatrix[1].insert(_DeqMatrix[1].begin(), -1);
//         }
//     }
//     _DeqMatrix.erase(_DeqMatrix.begin() + 1);
// }

// void    PmergeMe::DeqApplyMovesConquer(int index) {
//     for (size_t j = 0; j < _DeqMatrix[index + 1].size(); j++) {
//         int     valueInsert = _DeqMatrix[index + 1][j];
        
//         if (j >= _indexInsert.size())
//             _DeqMatrix[index].push_back(valueInsert);
//         else {
//             size_t  targetIndex = _indexInsert[j];
//             std::vector<int>::iterator pos = _DeqMatrix[index].begin() + targetIndex;
//             _DeqMatrix[index].insert(pos, valueInsert);
//         }
//     }
//     _DeqMatrix.erase(_DeqMatrix.begin() + index + 1);
// }