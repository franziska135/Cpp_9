#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#define ISFIRSTLINE 1
#define ISNOTFIRSTLINE 0

class BitcoinExchange {
    private:
        std::map<int, double>   Input;
        std::map<int, double>   Database;
        std::string             _database;
        bool                    _success;
        
    public:
        BitcoinExchange          (void);
        BitcoinExchange          (const BitcoinExchange& other);
        ~BitcoinExchange         ();

        BitcoinExchange&         operator=(const BitcoinExchange &other);
        
        void    loadFiles(std::string inputfile);
        void    readDatabase(std::string database, bool firstline);
        void    validLine(std::string line, bool firstline);
        void    checkDate(std::string date);
        void    checkPrice(double price);
        void    checkPrintKey(int key, double amount);

        class   FileError : public std::exception {
            public:
                const char * what() const throw();
        };

        class   DatabaseError : public std::exception {
            public:
                const char * what() const throw();
        };

        class   HeaderWrong : public std::exception {
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