#include "PmergeMe.hpp"

PmergeMe::PmergeMe() { }

PmergeMe::PmergeMe(const PmergeMe& other) {
    *this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _VecArray = other._VecArray;
        _DeqArray = other._DeqArray;
        _MergeTarget = other._MergeTarget;
        _InsertTarget = other._InsertTarget;
        _InsertSourceIndex = other._InsertSourceIndex;        
        _VecMatrix = other._VecMatrix;
        _DeqMatrix = other._DeqMatrix;
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
    for (size_t i = 0; i < _VecMatrix.size(); i++) {
        for (size_t j = 0; j < _VecMatrix[i].size(); j++)
            std::cout << "\t" << _VecMatrix[i][j] << " ";
        std::cout << std::endl;
    } std::cout << std::endl;
}

void    PmergeMe::printVector() {
    size_t i = 0;

    if (_VecMatrix[0].empty()) {
        std::cout << "Vector is empty" << std::endl;
        return ;
    } while (i < 5 && i < _VecMatrix[0].size()) {
        std::cout << _VecMatrix[0][i] << " ";
        i++;
    } if (_VecMatrix[0].size() > 5)
        std::cout << "[...]";
    std::cout << std::endl;
}

int PmergeMe::calc(int n) {
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;
    int first = 0;
    int second = 1;
    int result;

    for (int i = 2; i <= n; ++i) {
        result = 2 * first + second;
        first = second;
        second = result;
    }
    return result;
}

bool PmergeMe::VecIsSortedAscending() {
    std::vector<int> vec = _VecMatrix[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1])
            return false;
    }
    return true;
}

bool PmergeMe::DeqIsSortedAscending() {
    std::deque<int> vec = _DeqMatrix[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1])
            return false;
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

/*--------------------------------------------*/
/*Ford Johnson: with vector container---------*/
/*Divide Recursion: going to rock bottom------*/
/*Conquer Recursion: going up from rock bottom*/
/*--------------------------------------------*/

/*first step: going down to rock bottom*/
void    PmergeMe::VecMergeRecursion() {
    if (_VecMatrix[0].size() == 2 && _VecMatrix[0][0] && _VecMatrix[0][1]
        && _VecMatrix[0][0] > _VecMatrix[0][1]) {
            for (int i = _VecMatrix.size() - 1; i >= 0; i--)
                std::swap(_VecMatrix[i][0], _VecMatrix[i][1]);
        }
    if (_VecMatrix[0].size() <= 2)
        return ;

    if (_VecMatrix.size() < 2)
        VecMerge(0);
    else {
        VecMerge(0);
        for (int i = _VecMatrix.size() - 1; i > 1; i--)
            VecApplyMovesMerge(i);
    }

    if (!_MergeTarget.empty())
        _MergeTarget.clear();
    VecMergeRecursion();
}

/*second step: going up from rock bottom*/
void    PmergeMe::VecInsertRecursion() {
    if (_VecMatrix.size() == 1)
        return ;
    
    VecInsert();
    for (int i = _VecMatrix.size() - 2; i > 0; i -= 2)
        VecApplyMovesInsert(i);

    if (!_InsertTarget.empty())
        _InsertTarget.clear();
    if (!_InsertSourceIndex.empty())
        _InsertSourceIndex.clear();
    VecInsertRecursion();
}

/*implementations*/
/*dividing the upper std::vector element in the matrix and recording the steps*/
void    PmergeMe::VecMerge(int index) {
    std::vector <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (_VecMatrix[index][i] > _VecMatrix[index][i + 1]) {
            bigger.push_back(_VecMatrix[index][i]);
            smaller.push_back(_VecMatrix[index][i + 1]);
            _MergeTarget.push_back(FIRSTUP);
        } else {
            bigger.push_back(_VecMatrix[index][i + 1]);
            smaller.push_back(_VecMatrix[index][i]);
            _MergeTarget.push_back(SECONDUP);
        }
    } if (_VecMatrix[index].size() % 2 != 0)
        smaller.push_back(_VecMatrix[index].back());

    if (!_VecMatrix[index].empty())
    _VecMatrix.erase(_VecMatrix.begin() + index);
    _VecMatrix.insert(_VecMatrix.begin() + index, smaller);
    _VecMatrix.insert(_VecMatrix.begin() + index, bigger);
}

/*applying the recorded steps to the rest of std::vector matrix for pairs to stay tracable*/
void    PmergeMe::VecApplyMovesMerge(int index) {
    std::vector <int> bigger, smaller;
    for (size_t i = 0; i + 1 < _VecMatrix[index].size(); i += 2) {
        if (i / 2 >= _MergeTarget.size()) {
            while (i < _VecMatrix[index].size()) {
                smaller.push_back(_VecMatrix[index][i]);
                i++;
            }
        }
        else  {
            if (_MergeTarget[i / 2] == FIRSTUP) {
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

int     PmergeMe::VecAllDummy(int index){
    size_t i = 0;
    while (i < _VecMatrix[index].size()) {
        if (_VecMatrix[index][i] != -1 && _VecMatrix[index][i] != -2)
            return 0;
        i++;
    }
    return 1;
}


int PmergeMe::VecAmtofDummies(int j) {
    int count = 0;
    while (j >= 0) {
        if (_VecMatrix[1][j] == -2)
            count++;
        j--;
    }
    return count;
}
/*merging the upper two std::vector elements in the matrix and recording the steps*/
void    PmergeMe::VecInsert() {

    std::vector<int>::iterator pos;

    _VecMatrix[0].insert(_VecMatrix[0].begin(), _VecMatrix[1][0]);
    _VecMatrix[1][0] = -1;
    _VecMatrix[1].insert(_VecMatrix[1].begin(), -2);
    _InsertTarget.push_back(0);
    _InsertSourceIndex.push_back(0);

    int     dummy = 1;
    int     n = 2;
    int     start = calc(n) + dummy;
    int     end = 0;

    while (!VecAllDummy(1)) {
        for (int j = start; j > end; j--) {
            if (_VecMatrix[1][j] != -1 && _VecMatrix[1][j] != -2) {
                int trueIndex = j - VecAmtofDummies(j);
                int valueInsert = _VecMatrix[1][j];

                pos = std::lower_bound(_VecMatrix[0].begin(), _VecMatrix[0].begin() + j, valueInsert);
                int targetIndex = std::distance(_VecMatrix[0].begin(), pos);

                _InsertSourceIndex.push_back(trueIndex);
                _InsertTarget.push_back(targetIndex);

                _VecMatrix[0].insert(pos, valueInsert);
                _VecMatrix[1][j] = -1;

                _VecMatrix[1].insert(_VecMatrix[1].begin() + targetIndex, -2);
                dummy++;
            }
        }
        n++;
        start = calc(n) + dummy;
        if (start >= static_cast<int>(_VecMatrix[1].size()))
            start = static_cast<int>(_VecMatrix[1].size() - 1);
    }
    _VecMatrix.erase(_VecMatrix.begin() + 1);
}

/*applying the recorded stepts to the rest of std::vector matrix*/
void    PmergeMe::VecApplyMovesInsert(int index) {
    size_t targetSize = _InsertTarget.size();
    size_t sourceSize = _InsertSourceIndex.size();
    size_t j = 0;

    while (j < _VecMatrix[index + 1].size()) {
        if (j >= targetSize || j >= sourceSize) {
            while (j < _VecMatrix[index + 1].size()) {
                _VecMatrix[index].push_back(_VecMatrix[index + 1][j]);
                j++;
            }
            break ;
        }
        else {
            size_t  sourceIndex = _InsertSourceIndex[j];
            size_t  targetIndex = _InsertTarget[j];
            int     valueInsert = _VecMatrix[index + 1][sourceIndex];
            std::vector<int>::iterator pos = _VecMatrix[index].begin() + targetIndex;
            _VecMatrix[index].insert(pos, valueInsert);
        }
        j++;
    }
    _VecMatrix.erase(_VecMatrix.begin() + index + 1);
}

/*-----------------------------------*/
/*Ford Johnson: with deque container-*/
/*-----------------------------------*/

/*first step: going down to rock bottom*/
void    PmergeMe::DeqMergeRecursion() {
    if (_DeqMatrix[0].size() == 2 && _DeqMatrix[0][0] && _DeqMatrix[0][1]
        && _DeqMatrix[0][0] > _DeqMatrix[0][1]) {
            for (int i = _DeqMatrix.size() - 1; i >= 0; i--)
                std::swap(_DeqMatrix[i][0], _DeqMatrix[i][1]);
        }
    if (_DeqMatrix[0].size() <= 2)
        return ;

    if (_DeqMatrix.size() < 2)
        DeqMerge(0);
    else {
        DeqMerge(0);
        for (int i = _DeqMatrix.size() - 1; i > 1; i--)
            DeqApplyMovesMerge(i);
    }

    if (!_MergeTarget.empty())
        _MergeTarget.clear();
    DeqMergeRecursion();
}

/*second step: going up from rock bottom*/
void    PmergeMe::DeqInsertRecursion() {
    if (_DeqMatrix.size() == 1)
        return ;
    
    DeqInsert();
    for (int i = _DeqMatrix.size() - 2; i > 0; i -= 2)
        DeqApplyMovesInsert(i);

    if (!_InsertTarget.empty())
        _InsertTarget.clear();
    if (!_InsertSourceIndex.empty())
        _InsertSourceIndex.clear();
    DeqInsertRecursion();
}

/*implementations*/
/*dividing the upper std::deque element in the matrix and recording the steps*/
void    PmergeMe::DeqMerge(int index) {
    std::deque <int> bigger, smaller;

    for (size_t i = 0; i + 1 < _DeqMatrix[index].size(); i += 2) {
        if (_DeqMatrix[index][i] > _DeqMatrix[index][i + 1]) {
            bigger.push_back(_DeqMatrix[index][i]);
            smaller.push_back(_DeqMatrix[index][i + 1]);
            _MergeTarget.push_back(FIRSTUP);
        } else {
            bigger.push_back(_DeqMatrix[index][i + 1]);
            smaller.push_back(_DeqMatrix[index][i]);
            _MergeTarget.push_back(SECONDUP);
        }
    } if (_DeqMatrix[index].size() % 2 != 0)
        smaller.push_back(_DeqMatrix[index].back());

    if (!_DeqMatrix[index].empty())
    _DeqMatrix.erase(_DeqMatrix.begin() + index);
    _DeqMatrix.insert(_DeqMatrix.begin() + index, smaller);
    _DeqMatrix.insert(_DeqMatrix.begin() + index, bigger);
}

/*applying the recorded steps to the rest of std::deque matrix for pairs to stay tracable*/
void    PmergeMe::DeqApplyMovesMerge(int index) {
    std::deque <int> bigger, smaller;
    for (size_t i = 0; i + 1 < _DeqMatrix[index].size(); i += 2) {
        if (i / 2 >= _MergeTarget.size()) {
            while (i < _DeqMatrix[index].size()) {
                smaller.push_back(_DeqMatrix[index][i]);
                i++;
            }
        }
        else  {
            if (_MergeTarget[i / 2] == FIRSTUP) {
                bigger.push_back(_DeqMatrix[index][i]);
                smaller.push_back(_DeqMatrix[index][i + 1]);
            } else {
                bigger.push_back(_DeqMatrix[index][i + 1]);
                smaller.push_back(_DeqMatrix[index][i]);
            }
            if (!_DeqMatrix[1].empty() && (_DeqMatrix[1].size() % 2 != 0 || (_DeqMatrix[1].size() + _DeqMatrix[0].size()) % 2 != 0)
            && i + 3 == _DeqMatrix[index].size())
                smaller.push_back(_DeqMatrix[index][i + 2]);
        }
    }
    if (!_DeqMatrix[index].empty())
        _DeqMatrix.erase(_DeqMatrix.begin() + index);
    _DeqMatrix.insert(_DeqMatrix.begin() + index, smaller);
    _DeqMatrix.insert(_DeqMatrix.begin() + index, bigger);
}

int     PmergeMe::DeqAllDummy(int index){
    size_t i = 0;
    while (i < _DeqMatrix[index].size()) {
        if (_DeqMatrix[index][i] != -1 && _DeqMatrix[index][i] != -2)
            return 0;
        i++;
    }
    return 1;
}

int PmergeMe::DeqAmtofDummies(int j) {
    int count = 0;
    while (j >= 0) {
        if (_DeqMatrix[1][j] == -2)
            count++;
        j--;
    }
    return count;
}
/*merging the upper two std::deque elements in the matrix and recording the steps*/
void    PmergeMe::DeqInsert() {

    std::deque<int>::iterator pos;

    _DeqMatrix[0].insert(_DeqMatrix[0].begin(), _DeqMatrix[1][0]);
    _DeqMatrix[1][0] = -1;
    _DeqMatrix[1].insert(_DeqMatrix[1].begin(), -2);
    _InsertTarget.push_back(0);
    _InsertSourceIndex.push_back(0);

    int     dummy = 1;
    int     n = 2;
    int     start = calc(n) + dummy;
    int     end = 0;

    while (!DeqAllDummy(1)) {
        for (int j = start; j > end; j--) {
            if (_DeqMatrix[1][j] != -1 && _DeqMatrix[1][j] != -2) {
                int trueIndex = j - DeqAmtofDummies(j);
                int valueInsert = _DeqMatrix[1][j];

                pos = std::lower_bound(_DeqMatrix[0].begin(), _DeqMatrix[0].begin() + j, valueInsert);
                int targetIndex = std::distance(_DeqMatrix[0].begin(), pos);

                _InsertSourceIndex.push_back(trueIndex);
                _InsertTarget.push_back(targetIndex);

                _DeqMatrix[0].insert(pos, valueInsert);
                _DeqMatrix[1][j] = -1;

                _DeqMatrix[1].insert(_DeqMatrix[1].begin() + targetIndex, -2);
                dummy++;
            }
        }
        n++;
        start = calc(n) + dummy;
        if (start >= static_cast<int>(_DeqMatrix[1].size()))
            start = static_cast<int>(_DeqMatrix[1].size() - 1);
    }
    _DeqMatrix.erase(_DeqMatrix.begin() + 1);
}

/*applying the recorded stepts to the rest of std::deque matrix*/
void    PmergeMe::DeqApplyMovesInsert(int index) {
    size_t targetSize = _InsertTarget.size();
    size_t sourceSize = _InsertSourceIndex.size();
    size_t j = 0;

    while (j < _DeqMatrix[index + 1].size()) {
        if (j >= targetSize || j >= sourceSize) {
            while (j < _DeqMatrix[index + 1].size()) {
                _DeqMatrix[index].push_back(_DeqMatrix[index + 1][j]);
                j++;
            }
            break ;
        }
        else {
            size_t  sourceIndex = _InsertSourceIndex[j];
            size_t  targetIndex = _InsertTarget[j];
            int     valueInsert = _DeqMatrix[index + 1][sourceIndex];
            std::deque<int>::iterator pos = _DeqMatrix[index].begin() + targetIndex;
            _DeqMatrix[index].insert(pos, valueInsert);
        }
        j++;
    }
    _DeqMatrix.erase(_DeqMatrix.begin() + index + 1);
}
