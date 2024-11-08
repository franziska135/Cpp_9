#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(void) {
    _database = "data.csv";
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
    *this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        this->Input = other.Input;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange  () {
}

const char* BitcoinExchange::DatabaseError::what() const throw () {
    return "Database cannot be opened";
}

const char* BitcoinExchange::HeaderWrong::what() const throw () {
    return "Header is formatted incorrectly";
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

const char* BitcoinExchange::FutureDate::what() const throw () {
    return "Date is in the future";
}

//verify each line in the database;
//if not valid, no error but no storage of data
void    BitcoinExchange::readDatabase(std::string line, bool firstline) {
    size_t posPipe = line.find(',');
    if (posPipe == std::string::npos || posPipe == 0
        || posPipe == line.length() - 1
        || line.find('|', posPipe + 1) != std::string::npos) {
            if (firstline)
                throw HeaderWrong();
            else
                return ;
        }
    std::string date = line.substr(0, posPipe);
    std::string price = line.substr(posPipe + 1, line.length());
    
    //erase whitespaces from date and price
    date.erase(date.find_last_not_of(" \t") + 1);
    date.erase(0, date.find_first_not_of(" \t"));
    price.erase(price.find_last_not_of(" \t") + 1);
    price.erase(0, price.find_first_not_of(" \t"));
    
    if (firstline) {
        // std::cout << "Error:\tDatabase header formatted incorrectly" << std::endl;
        if (date != "date" || price != "exchange_rate")
            throw HeaderWrong();
        return ;
    }

    //price: check for content
    //date: check for space pos[length()] and dash at pos[4] and pos[7]
    if (price.empty() || date.length() != 10 || date[4] != '-' || date[7] != '-')
        return ;

    //date::erase dashes
    std::string dateCpy = date;
    date.erase(7,1);
    date.erase(4,1);
    
    //check the date (format, valid date)
    int     dateVal;
    double  priceVal;

    try {
        checkDate(date);
        std::istringstream(date) >> dateVal;
    }
    catch (std::exception &e) {
        return ;
    }

    //check the price is a valid double
    try {
        std::istringstream iss(price);
        if (!(iss >> priceVal))
            throw badInput();
        std::string remnant;
        if (iss >> remnant)
            throw badInput();
    
        if (priceVal < 0 || priceVal > 2147483647)
            throw badInput();
    }
    catch (std::exception &e) {
        return ;
    }
    this->Database[dateVal] = priceVal;
}

void    BitcoinExchange::checkPrice(double price) {
    if (price < 0 || price > 1000)
        throw badInput();
}

//verifies the validity of the date: numeric, valid dates, consider leap years
void    BitcoinExchange::checkDate(std::string date) {
    for (size_t i = 0; i < date.length(); ++i) {
        if (!std::isdigit(date[i]))
            throw badInput();
    }
    
    int year, month, day;

    std::string yearStr = date.substr(0, 4);
    std::string monthStr = date.substr(4, 2);
    std::string dayStr = date.substr(6, 2);

    std::istringstream(yearStr) >> year;
    std::istringstream(monthStr) >> month;
    std::istringstream(dayStr) >> day;
        
    if (year < 2009)
        throw badInput();
    if (month < 1 || month > 12)
        throw badInput();
    if (day < 1 || day > 31)
        throw badInput();

    std::time_t now = std::time(0);
    std::tm* localT = std::localtime(&now);

    int todayDay = localT->tm_mday;
    int todayMonth = localT->tm_mon + 1;
    int todayYear = localT->tm_year + 1900;

    if (year > todayYear || (year == todayYear && (month > todayMonth || (month == todayMonth && day > todayDay))))
        throw FutureDate();
    
    int monthDay [] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (day > monthDay[month - 1])
        if (!(month == 2 && day == 29 && (year % 4 == 0 || (year % 100 != 0 && year % 400 == 0))))
            throw badInput();
}

//Determines the correct output: the amount of btc * the price at the given date
//or if not found most recent date
//error if no most recent date found (if no date before the wanted one is stored in database)
void    BitcoinExchange::checkPrintKey(int date, double amount) {
    std::map<int, double>::iterator it = Database.find(date);

    double finalPrice = 0;

    if (it != Database.end()) {
        finalPrice = amount * it->second;
        std::cout << finalPrice << std::endl;
    }
    else {
        it = Database.lower_bound(date);

        if (it == Database.begin() && it->first > date)
            std::cout << "date is too early, no entries found in database" << std::endl;
        else {
            if (it == Database.end() || it->first > date)
                --it;
            finalPrice = amount * it->second;
            std::cout << finalPrice << std::endl;
        }

    }
}

//iterated over each line of the Input and verifies that all rules are adhered to
void    BitcoinExchange::validLine(std::string line, bool firstline) {
    size_t posPipe = line.find('|');
    if (posPipe == std::string::npos || posPipe == 0
        || posPipe == line.length() - 1
        || line.find('|', posPipe + 1) != std::string::npos) {
            if (firstline)
                throw HeaderWrong();
            else
                throw invalidLine();
        }
    
    std::string date = line.substr(0, posPipe);
    std::string value = line.substr(posPipe + 1, line.length());
    
    //erase whitespaces from date and value
    date.erase(date.find_last_not_of(" \t") + 1);
    date.erase(0, date.find_first_not_of(" \t"));

    value.erase(value.find_last_not_of(" \t") + 1);
    value.erase(0, value.find_first_not_of(" \t"));

    //value: check for content
    //date: check for space pos[length()] and dash at pos[4] and pos[7]
    if (firstline) {
        // std::cout << "Error:\tInput header formatted incorrectly" << std::endl;
        if (date != "date" || value != "value")
            throw HeaderWrong();
        return ;
    }
    if (value.empty() || date.length() != 10 || date[4] != '-' || date[7] != '-')
        throw invalidLine();

    //date: erase dashes
    std::string dateCpy = date;
    date.erase(7,1);
    date.erase(4,1);
    
    //verifies validity of date
    int     dateVal;
    double  amountVal;

    try {
        checkDate(date);
        std::istringstream(date) >> dateVal;
    }
    catch (std::exception &e) {
        std::cout << "Error:\t" << e.what() << " => " << dateCpy << std::endl;
        _success = false;
        return ;
    }

    //check the price is a valid double
    try {
        std::istringstream iss(value);
        if (!(iss >> amountVal))
            throw badInput();
        std::string remnant;
        if (iss >> remnant)
            throw badInput();
        checkPrice(amountVal);
    }
    catch (std::exception &e) {
        std::cout << "Error:\t" << e.what() << " => " << value << std::endl;
        _success = false;
        return ;
    }
    this->Input[dateVal] = amountVal;
    
    std::cout << dateCpy << " => " ;
    checkPrintKey(dateVal, amountVal);
    return ;
}


void BitcoinExchange::loadFiles(std::string input) {
    std::ifstream iFile;
    std::ifstream database;

    //both datatbases should open, terminate if they don't
    iFile.open (input.c_str());
    if (!iFile.is_open())
        throw FileError();

    database.open (this->_database.c_str());
    if (!database.is_open())
        throw DatabaseError();
    
    std::string line;

    //both headers should be valid,t terminate if they are not
    //iterate over each line in each database; program does not terminate in error cases
    std::getline(database, line);
    readDatabase(line, ISFIRSTLINE);
    while (std::getline(database, line))
        readDatabase(line, ISNOTFIRSTLINE);

    std::getline(iFile, line);
    validLine(line, ISFIRSTLINE);
    while (std::getline(iFile, line)) {
        try {
            validLine(line, ISNOTFIRSTLINE);
        } catch (std::exception &e) {
            std::cout << "Error:\t" << e.what() << std::endl;
            _success = false;
        }
    }
}