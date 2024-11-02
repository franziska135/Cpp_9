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

void    PmergeMe::printVector(int index) {

    if (_VecMatrix[index].empty()) {
        std::cout << "Vector is empty" << std::endl;
        return ;
    }
    // size_t i = 0;
    // while (i < v.size()) {
    //     std::cout << v[i] << " ";
    //     i++;
    // }
    // std::cout << std::endl;
    int limit = _VecMatrix[index].size(), i = 0;
    
    if (_VecMatrix[index].size() > 5)
        limit = 5;
    while (i < limit - 1) {
        std::cout << _VecMatrix[index][i] << " ";
        i++;
    } std::cout << _VecMatrix[index][_VecMatrix[index].size() - 1];
    
    if (_VecMatrix[index].size() > 5)
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

void    PmergeMe::separateDivide(int index) {
    std::vector <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (_VecMatrix[index][i] > _VecMatrix[index][i + 1]) {
            bigger.push_back(_VecMatrix[index][i]);
            smaller.push_back(_VecMatrix[index][i + 1]);
            _moves.push_back(FIRSTUP);
            // std::cout << "Firstup" << std::endl;
        } else {
            bigger.push_back(_VecMatrix[index][i + 1]);
            smaller.push_back(_VecMatrix[index][i]);
            _moves.push_back(SECONDUP);
            // std::cout << "secondup" << std::endl;

        }
    } if (_VecMatrix[index].size() % 2 != 0) {
        // std::cout << "this is a leftover in front line: " << _VecMatrix[index].back() <<  "in matrix index: " << index << std::endl;
        smaller.push_back(_VecMatrix[index].back());
    }

    if (!_VecMatrix[index].empty())
    _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
    // printMatrix();
    // std::cout << "\nvector after round of divide:\n";
    // printMatrix();
}

void    PmergeMe::applyMovesDivide(int index) {
    std::vector <int> bigger, smaller;

    //  std::cout << "applying the moves from frontline on matrix index: " << index << std::endl;
    // printMatrix();
    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        // std::cout << "i: " << i << std::endl;
        // std::cout << "vec index: " << index << ", full size: " <<  _VecMatrix[index].size() << std::endl;
        if (_moves[i / 2] == FIRSTUP) {
            bigger.push_back(_VecMatrix[index][i]);
            smaller.push_back(_VecMatrix[index][i + 1]);
            // std::cout << "firstup " << std::endl;
        } else {
            bigger.push_back(_VecMatrix[index][i + 1]);
            smaller.push_back(_VecMatrix[index][i]);
            // std::cout << "secondup" << std::endl;
        }
        size_t iCpy = i;
        iCpy +=2;
        if (!_VecMatrix[1].empty() && (_VecMatrix[1].size() % 2 != 0 || (_VecMatrix[1].size() + _VecMatrix[0].size()) % 2 != 0)
        && iCpy + 1 == _VecMatrix[index].size()) {
            // std::cout << "this is a leftover: " << _VecMatrix[index].back() << std::endl;
            // std::cout << "leftover left in place";
            while (iCpy < _VecMatrix[index].size()) {
                // std::cout << "added to smaller: " << _VecMatrix[index][iCpy] << std::endl;
                smaller.push_back(_VecMatrix[index][iCpy]);
                iCpy++;
            }
            break ;
        }
    } 
    // std::cout << std::endl;
    // printMatrix();
    // if (!_VecMatrix[1].empty() && _VecMatrix[1].size() % 2 != 0) {
    //     std::cout << "this is a leftover: " << _VecMatrix[index].back() << std::endl;
    //     smaller.push_back(_VecMatrix[index].back());
    // }
    if (!_VecMatrix[index].empty())
        _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
    // std::cout << "matrix after apply moves and insertion" << std::endl;
    // printMatrix();

}

void    PmergeMe::DivideRecursion() {

    if (_VecMatrix[0].size() == 2 && _VecMatrix[0][0] && _VecMatrix[0][1]
        && _VecMatrix[0][0] > _VecMatrix[0][1]) {
            for (int i = _VecMatrix.size() - 1; i >= 0; i--)
                std::swap(_VecMatrix[i][0], _VecMatrix[i][1]);
        }
    if (_VecMatrix[0].size() <= 2)
        return ;
    
    separateDivide(0);
    for (int i = _VecMatrix.size() - 1; i > 1; i--)
        applyMovesDivide(i);

    if (!_moves.empty())
        _moves.clear();
    // std::cout << "finished one round of divide" << std::endl;
    // printMatrix();
    DivideRecursion();
}

// bool    PmergeMe::allElementsProcessed(int index) {
//     for (size_t j = 0; j < _VecMatrix[index].size(); ++j) {
//             if (_VecMatrix[index][j] != -1)
//                 return false ;
//     }
//     return true ;
// }

void    PmergeMe::ConquerMerge() {
    // std::cout << "Registering moves" << std::endl;

    // std::cout << "indices b4 registering: "<<std::endl;

    // for (size_t i = 0; i < _indexInsert.size(); i++)
    //     std::cout << _indexInsert[i] << " ";

    std::vector<int>::iterator pos;
    // pos = std::lower_bound(_VecMatrix[0].begin(), _VecMatrix[0].end(), _VecMatrix[1][0]);
    // int index = std::distance(_VecMatrix[0].begin(), pos);
    // _indexInsert.push_back(index);
    // _VecMatrix[0].insert(pos, _VecMatrix[1][0]);
    // _VecMatrix[1][0] = -1;
    // _VecMatrix[1].insert(_VecMatrix[1].begin(), -1);
    
    _VecMatrix[0].insert(_VecMatrix[0].begin(), _VecMatrix[1][0]);
    _VecMatrix[1][0] = -1;
    _VecMatrix[1].insert(_VecMatrix[1].begin(), -1);
    _indexInsert.push_back(0);
    
    std::cout << "FRONTLINE: first mindless push in front abled" << std::endl;
    // printMatrix();
    
    for (size_t i = 0; i < _VecMatrix[1].size(); i++) {
        if (_VecMatrix[1][i] != -1) {
            int valueInsert = _VecMatrix[1][i];
            // std::cout << "value to insert: " << valueInsert << std::endl;
            // std::cout << "searching index 0 - " << i + 1 << std::endl;
            if (i < _VecMatrix[0].size() && _VecMatrix[0][i])
                pos = std::lower_bound(_VecMatrix[0].begin(), _VecMatrix[0].begin() + i, valueInsert);
            else
                pos = std::lower_bound(_VecMatrix[0].begin(), _VecMatrix[0].end(), valueInsert);
            int index = std::distance(_VecMatrix[0].begin(), pos);
            // std::cout << "value insert at position: " << index << std::endl;
            _VecMatrix[0].insert(pos, valueInsert);
            _indexInsert.push_back(index);
            _VecMatrix[1][i] = -1;
            _VecMatrix[1].insert(_VecMatrix[1].begin(), -1);
            // std::cout << "matrix after insertion: " << std::endl;
            // printMatrix();
        }
    }
    // std::cout << std::endl;
    _VecMatrix.erase(_VecMatrix.begin() + 1);
    // std::cout << "indices: "<<std::endl;

    // for (size_t i = 0; i < _indexInsert.size(); i++)
    //     std::cout << _indexInsert[i] << " ";
    std::cout << std::endl;
}

void    PmergeMe::applyMovesConquer(int i) {
    // _VecMatrix[i].insert(_VecMatrix[i].begin(), _VecMatrix[i + 1][0]);
    // _VecMatrix[i + 1][0] = -1;
    // _VecMatrix[i + 1].insert(_VecMatrix[i + 1].begin(), -1);
    
    /*make it latant*/
    int keepTrackOfInsertions = 0;

    for (size_t j = 0; j < _VecMatrix[i + 1].size(); j++) {
        
        int valueInsert = _VecMatrix[i + 1][j];
        if (valueInsert != -1) {
            int targetIndex = _indexInsert[keepTrackOfInsertions];
            std::vector<int>::iterator pos = _VecMatrix[i].begin() + targetIndex;
            if ((size_t)_indexInsert[keepTrackOfInsertions] >= _VecMatrix[i].size()) {
                // pos = std::lower_bound(_VecMatrix[i].begin(), _VecMatrix[i].end(), valueInsert);
                // pos = _VecMatrix[i].end();
                _VecMatrix[i].push_back(valueInsert);
                std::cout << "a problem happened" << std::endl;
            std::cout << "value to insert: " << valueInsert << std::endl;
            
            std::cout << "position: " << _indexInsert[keepTrackOfInsertions] << std::endl;
            int poscpy = std::distance(_VecMatrix[i].begin(), pos);
            std::cout << "new pos: " << poscpy << std::endl;
            std::cout << "size of vec where insert happens: " << _VecMatrix[i].size() << std::endl;
            std::cout << std::endl;
            }
            else {
                _VecMatrix[i].insert(pos, valueInsert);
            }
            _VecMatrix[i + 1][j] = -1;
            _VecMatrix[i + 1].insert(_VecMatrix[i + 1].begin(), -1);
            keepTrackOfInsertions++;
        }
    }
    
    _VecMatrix.erase(_VecMatrix.begin() + i + 1);
}

void    PmergeMe::ConquerRecursion() {
    std::cout << std::endl;
    // printMatrix();
    
    if (_VecMatrix.size() == 1)
        return ;
    ConquerMerge();
    for (int i = _VecMatrix.size() - 2; i > 0; i -= 2)
        applyMovesConquer(i);
    
    // std::cout << "after one conquer: " << std::endl;
    // printMatrix();
    if (!_indexInsert.empty())
        _indexInsert.clear();
    ConquerRecursion();
}

void PmergeMe::printMatrix() {
    for (size_t i = 0; i < _VecMatrix.size(); i++) {
        for (size_t j = 0; j < _VecMatrix[i].size(); j++)
            std::cout << "\t" << _VecMatrix[i][j] << " ";
        std::cout << std::endl;
    } std::cout << std::endl;
}

bool PmergeMe::isSortedAscending() {
    std::vector<int> vec = _VecMatrix[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) {
            return false;
        }
    }
    return true;
}