
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

#include <iostream>
#include <sstream>

#include "biginteger.h"

BigIntegerVersionInfo& BigIntegerVersionInfo::Instance() {
    static BigIntegerVersionInfo s;
    return s;
}

int BigIntegerVersionInfo::getMajor() {
    return major;
}

int BigIntegerVersionInfo::getMinor() {
    return minor;
}

int BigIntegerVersionInfo::getBuild() {
    return build;
}

std::string BigIntegerVersionInfo::getName() {
    return name;
}

std::string BigIntegerVersionInfo::getLicense() {
    return license;
}

std::string BigIntegerVersionInfo::getCompleteInfo() {
    std::ostringstream s;
    s << "\n" << name << " version " << 
        major << "." << minor << "." << build << " " << license << "\n";
    #ifdef BUILD
        s << "Target " << BUILD << std::endl;
    #endif
    s << "Build at " << __DATE__ << " " << __TIME__ << std::endl << std::endl;
    return s.str();
}

void BigIntegerVersionInfo::printCompleteInfo() {
    std::cout << getCompleteInfo();
}
