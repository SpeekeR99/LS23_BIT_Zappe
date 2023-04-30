
/*
    BigInteger - arbitrary-precision integer library 
    Copyright (C) 2012-2013, 2018 Yaroslav Zotov.
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BIGINTEGER_EXCEPTION_HEADER
#define BIGINTEGER_EXCEPTION_HEADER

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

// Exception class for bignumbers
class BigIntegerException : public std::exception {
public:
    // Log error before throwing
    void LogError(bool show) {
        if (show)
            std::cerr << report << std::endl;
    }

    // Construct an a exception
    BigIntegerException(const char* message, const char* file, int line)
        : file(file)
        , line(line)
        , message(message) {
        std::ostringstream outputStream;
        std::string messageString = message;

        if (!messageString.empty()) {
            outputStream << messageString;
        } else {
            outputStream << "Unknown exception";
        }
        outputStream << " in file '" << file << "' line " << line;
        report = outputStream.str();

        LogError(show);
    }

    // The assertion message
    virtual const char* what() const throw() {
        return report.c_str();
    }

    // Source file
    const char* File() const throw() {
        return file;
    }

    // Source line
    int Line() const throw() {
        return line;
    }

    // Description of failure
    const char* Message() const throw() {
        return message;
    }
    //Destructor needed for C++03 standard
    ~BigIntegerException() throw() {};
private:
    const char* file;
    int line;
    const char* message;
    std::string report;
    #ifdef DEBUG_EXCEPTIONS
        static const bool show = true;
    #else
         static const bool show = false;
    #endif
};

#endif
