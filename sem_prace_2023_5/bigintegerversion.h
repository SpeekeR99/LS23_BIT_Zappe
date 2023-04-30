
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

#ifndef BIGINTEGER_VERSION_HEADER
#define BIGINTEGER_VERSION_HEADER

#include <string>

//Singleton for version info
class BigIntegerVersionInfo {
public:
    static BigIntegerVersionInfo& Instance();
    int getMajor();
    int getMinor();
    int getBuild();
    std::string getCompleteInfo();
    std::string getLicense();
    std::string getName();
    void printCompleteInfo();

private:
    int major, minor, build;
    std::string license, name;
    //copies, constructors and destructors are forbidden
    BigIntegerVersionInfo() {
        //init version data:
        major = 1;
        minor = 0;
        build = 4;
        name = "BigInteger library";
        license = "Copyright (C) 2018 Yaroslav Zotov\n\
This program comes with ABSOLUTELY NO WARRANTY.\n\
This is free software, and you are welcome to redistribute it\n\
under certain conditions.\n";
    }
    ~BigIntegerVersionInfo() {}
#if __cplusplus > 201100
    BigIntegerVersionInfo(BigIntegerVersionInfo const&) = delete;
    BigIntegerVersionInfo& operator= (BigIntegerVersionInfo const&) = delete;
#else
    BigIntegerVersionInfo(BigIntegerVersionInfo const&);
    BigIntegerVersionInfo& operator= (BigIntegerVersionInfo const&);
#endif
};

#endif
