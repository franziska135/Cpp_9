#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

class BitcoinExchange {
    private:
        std::map<int, double>   Data;
        std::string             _database;
        bool                    _success;
        
    public:
        BitcoinExchange          (void);
        BitcoinExchange          (const BitcoinExchange& other);
        ~BitcoinExchange         ();

        BitcoinExchange&         operator=(const BitcoinExchange &other);
        
        void    loadFile(std::string inputfile);
        void    validLine(std::string line);
        // void    extractValue(std::string line, int &year, double &price);
        int     checkDate(std::string date);

        class   FileError : public std::exception {
            public:
                const char * what() const throw();
        };

        class   DatabaseError : public std::exception {
            public:
                const char * what() const throw();
        };

        class   invalidLine : public std::exception {
            public:
                const char * what() const throw();
        };

        class   badInput : public std::exception {
            public:
                const char * what() const throw();
        };
};


#endif