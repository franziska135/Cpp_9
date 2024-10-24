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

void BitcoinExchange::validLine(std::string line) {

    //check pipe
    size_t posPipe = line.find('|');
    if (posPipe == std::string::npos || posPipe == 0
        || posPipe == line.length() - 1
        || line.find('|', posPipe + 1) != std::string::npos)
        throw invalidLine();
    
    std::string date = line.substr(0, posPipe);
    std::string price = line.substr(posPipe + 1, line.length());
    
    //erase whitespaces from date and price
    date.erase(date.find_last_not_of(" \t") + 1);
    date.erase(0, date.find_first_not_of(" \t"));

    price.erase(price.find_last_not_of(" \t") + 1);
    price.erase(0, price.find_first_not_of(" \t"));
    
    
    //price: check for content
    //date: check for space pos[length()] and dash at pos[4] and pos[7]
    if (price.empty() || date.length() != 10 || date[4] != '-' || date[7] != '-')
        throw invalidLine();

    //date::erase dashes
    std::string dateCpy = date;
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
        std::istringstream(date) >> dateVal;
    }
    catch (std::exception &e) {
        std::cout << "Error:\t" << e.what() << " => " << dateCpy << std::endl;
        _success = false;
        return ;
    }

    try {
        std::istringstream iss(price);
        if (!(iss >> priceVal))
            throw badInput();
        std::string remnant;
        if (iss >> remnant)
            throw badInput();
    }
    catch (std::exception &e) {
        std::cout << "Error:\t" << e.what() << " => " << price << std::endl;
        _success = false;
        return ;
    }
    this->Data[dateVal] = priceVal;
    
    /*take out again*/
    std::cout << dateCpy << " | " << price << std::endl;
    
    return ;
}


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
    
    while (std::getline(iFile, line)) {
        try {
            validLine(line);
        } catch (std::exception &e) {
            std::cout << "Error:\t" << e.what() << std::endl;
            _success = false;
        }
    }
}


