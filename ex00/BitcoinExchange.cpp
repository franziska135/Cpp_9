#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void) {
    _database = "data.csv";
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        this->Data = other.Data;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange  () {
}

const char* BitcoinExchange::DatabaseError::what() const throw () {
    return "Database cannot be opened";
}

const char* BitcoinExchange::FileError::what() const throw () {
    return "Input file cannot be opened";
}

const char* BitcoinExchange::invalidLine::what() const throw () {
    return "Line not formatted correctly";
}

const char* BitcoinExchange::badInput::what() const throw () {
    return "Bad input";
}

int    BitcoinExchange::checkDate(std::string date) {
    for (size_t i = 0; i < date.length(); ++i) {
        if (!std::isdigit(date[i]))
            throw badInput();
    }
    return 1;
}

int BitcoinExchange::validLine(std::string line) {

    //check pipe
    size_t posPipe = line.find('|');
    if (posPipe == std::string::npos || posPipe == 0
        || posPipe == line.length() - 1
        || line.find('|', posPipe + 1) != std::string::npos)
        throw invalidLine();
    
    std::string date = line.substr(0, posPipe);
    std::string price = line.substr(posPipe + 1, line.length());
    
    std::cout << "'" << date << "' '" << price<< "'" << std::endl;

    //erase space from date and price
    date.erase(date.find_last_not_of(" \t") + 1);
    price.erase(0, price.find_first_not_of(" \t"));
    
    //price: check for content
    //date: check for space pos[length()] and dash at pos[4] and pos[7]
    if (price.empty() || date.length() != 10 || date[4] != '-' || date[7] != '-')
        throw invalidLine();

    //date::erase dashes
    date.erase(7,1);
    date.erase(4,1);
    
    //check the date
    /*inportant to find a solution to print the input*/
    int     dateVal;
    double  priceVal;

    /*can turn checkDate to return int and then throw appropriate error
    with a message (date) passed*/
    try {
        checkDate(date);
        dateVal = std::stoi(date);
    }
    catch (std::exception &e) {
        std::cout << "Error:\t\t" << e.what() << "=>" << date << std::endl;
        _success = false;
        return ;
    }

    try {
        priceVal = std::stod(price);
    }
    catch (std::exception &e) {
        std::cout << "Error:\t\t" << e.what() << "=>" << price << std::endl;
        _success = false;
        return ;
    }
    std::cout << "'" << date << "' '" << price<< "'" << std::endl;
    this->Data[dateVal] = priceVal;
    return ;
}

// void    BitcoinExchange::extractValue(std::string line, int &year, double &price) {
//     /*fill*/
// }

void BitcoinExchange::loadFile(std::string input) {
    std::ifstream iFile;
    std::ifstream database;

    iFile.open (input.c_str());
    if (!iFile.is_open())
        throw FileError();

    database.open (this->_database.c_str());
    if (!database.is_open())
        throw DatabaseError();
    
    std::string line;
    int         i = 0;
    
    while (std::getline(iFile, line)) {
        try {
            validLine(line);
        } catch (std::exception &e) {
            std::cout << "Error:\t\t" << e.what() << std::endl;
            _success = false;
        }
    }
}

void processData();


