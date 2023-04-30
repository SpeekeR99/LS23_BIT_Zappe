
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

#include <cstring>
#include <cstdlib>
#include <sstream>

#include "biginteger.h"

std::ostream& operator<<(std::ostream& s, const BigInteger& number) {
    //write BigInteger
    if (number.size == 0) { //number is zero
        s << "0";
        return s;
    }
    const int divValue = 6; //used for make required space for printed string
    long newLength = number.size + number.size/divValue + 1;
    digit *newLong = new digit [newLength];
    if (number.sign)
        s << "-";
    BigInteger A(number);
    for(long i = 0; i < newLength; i++)
        tinyDivide(A, A, BigInteger::Base2, newLong[i]);
    while (newLong[newLength - 1] == 0)
         newLength--;
    s << newLong[newLength - 1];
    for(long i = newLength - 2; i >= 0; i--) {
        digit temp = newLong[i], power = BigInteger::Base2 / 10;
        for (digit j = 0; j < BigInteger::Base2Log; j++) {
            s << temp / power;
            temp %= power;
            power /= 10;
        }
    }
    delete [] newLong;
    return s;
}

std::istream& operator>>(std::istream& s, BigInteger& number) {
    //read BigInteger
    std::string tmp;
    s >> tmp;
    number = tmp;
    return s;
}

BigInteger::BigInteger() {
    //Default constructor - init with zero
    sign = 0;
    size = 0;
    num.push_back(0);
}

#if __cplusplus > 201100
    BigInteger::BigInteger(const BigInteger &value) {
        this->num = value.num;
        this->size = value.size;
        this->sign = value.sign;
    }

    BigInteger::BigInteger(BigInteger&& value) {
        this->num = std::move(value.num);
        this->size = value.size;
        this->sign = value.sign;
    }

    BigInteger& BigInteger::operator=(BigInteger&& value) {
        this->num = std::move(value.num);
        this->size = value.size;
        this->sign = value.sign;
        return *this;
    }
#endif

void BigInteger::constructFromString(const char* s1) {
    while(s1[0] == ' ' || s1[0] == '\t')
        s1++;
    sign = (s1[0] == '-');
    if (sign)
        s1++;
    for (size_t i = 0; i < strlen(s1); i ++)
        if (s1[i] < '0' || s1[i] > '9')
           throw BigIntegerException((std::string("Bad characters. Can't parse string ") + s1 + " in BigInteger constructor").c_str(), __FILE__, __LINE__);
    while(s1[0] == '0')
        s1++;
    if(strlen(s1) == 0) {
        sign = 0;
        num.push_back(0);
        size = 0;
        return;
    }
    size = strlen(s1) / Base3Log + (strlen(s1)%Base3Log == 0 ? 0 : 1);
    long size2 = size + size/2 + 1;
    uLong* num1 = new uLong[size];
    for (long i = 0; i < size; i++)
        num1[i] = 0;
    long index = size - 1;
    uLong power = 1;
    for (long i = strlen(s1) - 1; i >= 0; i--) {
        num1[index] += (s1[i] - '0') * power;
        power *= 10;
        if (power == Base3) {
            index--;
            power = 1;
        }
    }
    num.resize(size2, 0);
    for(long i = 0; i < size2; i++)
        fromDecimal(num1, size, num[i]);
    delete [] num1;
    size = size2;
    this->deleteLeadingZeroes();
}

BigInteger::BigInteger(std::string s1) {
    this->constructFromString(s1.c_str());
}

BigInteger::BigInteger(const char* s1) {
    this->constructFromString(s1);
}

template<class T>
void BigInteger::constructFromSigned(T min_value, T value, T (*abs_func)(T)) {
    size = 0;
    sign = value < 0;
    if (value == 0) {
        num.push_back(0);
        return;
    }
    if (value == min_value) {
        uLong temp = -value;
        while (temp != 0) {
            temp >>= BigInteger::BaseLog;
            size++;
        }
        for (int i = size - 1; i >= 0; i--) {
            num.push_back(value & BigInteger::BaseMod);
            value >>= BigInteger::BaseLog;
        }
        return;
    }
    value = abs_func(value);
    T temp = value;
    while (temp != 0) {
        temp >>= BigInteger::BaseLog;
        size++;
    }
    for (int i = size - 1; i >= 0; i--) {
        num.push_back(value & BigInteger::BaseMod);
        value >>= BigInteger::BaseLog;
    }
}

BigInteger::BigInteger(long l) {
    this->constructFromSigned(LONG_MIN, l, labs);
}

BigInteger::BigInteger(long long l) {
    this->constructFromSigned(LLONG_MIN, l, llabs);
}

BigInteger::BigInteger(int l) {
    this->constructFromSigned(INT_MIN, l, abs);
}

template<class T>
void BigInteger::constructFromUnsigned(T value) {
    size = 0;
    sign = value < 0;
    if (value == 0) {
        num.push_back(0);
        return;
    }
    T temp = value;
    while (temp != 0) {
        temp >>= BigInteger::BaseLog;
        size++;
    }
    for (int i = size - 1; i >= 0; i--) {
        num.push_back(value & BigInteger::BaseMod);
        value >>= BigInteger::BaseLog;
    }
}

BigInteger::BigInteger(digit l) {
    this->constructFromUnsigned(l);
}

BigInteger::BigInteger(uLong l) {
    this->constructFromUnsigned(l);
}

BigInteger::BigInteger(unsigned long l) {
    this->constructFromUnsigned(l);
}

BigInteger& BigInteger::operator=(const BigInteger &n1){
    if (this == &n1)
        return *this;
    sign = n1.sign;
    size = n1.size;
    this->num = n1.num;
    return *this;
}

BigInteger abs(const BigInteger &n1) {
    BigInteger temp(n1);
    temp.sign = false;
    return temp;
}

BigInteger BigInteger::operator-() const {
    BigInteger temp(*this);
    temp.sign = !temp.sign;
    return temp;
}

BigInteger BigInteger::operator+() const {
    return *this;
}

bool operator==(const BigInteger& n2, const BigInteger& n1) {
    if (n1.sign == n2.sign) {
        if (n1.size == n2.size) {
            const digit *n20 = &n2.num[0], *n10 = &n1.num[0];
            for (long i = 0; i < n2.size; i++)
                if (n10[i] != n20[i])
                    return false;
            return true;
        }
        return false;
    }
    return false;
}

bool operator!=(const BigInteger& n1, const  BigInteger& n2) {
    return !(n2 == n1);
}

bool operator>(const BigInteger& n1, const BigInteger& n2) {
    if (n2.sign && !n1.sign)
        return true;
    else if (n1.sign && !n2.sign)
        return false;
    if (n1.size == n2.size) {
        if (n1.sign)
            return abs(n2) > abs(n1);
        const digit *n10 = &n1.num[0], *n20 = &n2.num[0];
        for (long i = n1.size - 1; i >= 0; i--)
            if (n10[i] > n20[i])
                return true;
            else if (n10[i] < n20[i])
                return false;
    }
    if (n1.sign)
        return n1.size < n2.size;
    else
        return n1.size > n2.size;
}

bool operator<(const BigInteger& n1, const BigInteger& n2) {
    return n2 > n1;
}

bool operator>=(const BigInteger& n1, const BigInteger& n2) {
    return n2 == n1 || n1 > n2;
}

bool operator<=(const BigInteger& n1, const BigInteger& n2) {
    return n1 == n2 || n1 < n2;
}

bool absCompare(const BigInteger &n1, const BigInteger &n2) { //return true if abs(n1) > abs(n2)
    if (n1.size == n2.size) {
        const digit *n10 = &n1.num[0], *n20 = &n2.num[0];
        for (long i = n1.size - 1; i >= 0; i--)
            if (n10[i] > n20[i])
                return true;
            else if (n10[i] < n20[i])
                return false;
    }
    return n1.size > n2.size;
}

BigInteger operator+(const BigInteger &n0, const BigInteger &n1) {
    BigInteger result(0);
    bigAdd(n0, n1, result);
    return result;
}

//I use macros here because it's the most elegant way to make more beautiful code
#define ADD_LOOP(FIRST_FUNC, BaseMod, BaseLog) \
    while (++i < l1) {\
        carry += FIRST_FUNC; /*s1[i] + s0[++j];*/\
        ret[j] = carry & BaseMod;\
        carry >>= BaseLog;\
    }\
    while (carry && j < l0 - 1) {\
        carry += s0[++j];\
        ret[j] = carry & BaseMod;\
        carry >>= BaseLog;\
    }

#define CARRY_ADD s1[i] + s0[++j]
#define CARRY_SUB (long long)s0[++j] - s1[i]

void bigAdd(const BigInteger &n0, const BigInteger &n1, BigInteger &result) {
    if (absCompare(n1, n0)) {
        bigAdd(n1, n0, result);
        return;
    }
    long l0 = n0.size, l1 = n1.size, i = -1, j = -1;//i - l1, j - l0
    const digit *s0 = &n0.num[0], *s1 = &n1.num[0];
    if(result.size < l0) {
        result = n0;
    }
    digit *ret = &result.num[0];
    long long carry = 0;
    if (n1.sign == n0.sign) {
        ADD_LOOP(CARRY_ADD, BigInteger::BaseMod, BigInteger::BaseLog)
        if (carry) { //There was a carry so add it to the result number
            result.size++;
            result.num.push_back(carry);
        }
    } else {
        ADD_LOOP(CARRY_SUB, BigInteger::BaseMod, BigInteger::BaseLog)
        result.deleteLeadingZeroes(); //size of result may be less than operands
    }
}

void bigSub(const BigInteger &n0, const BigInteger &n1, BigInteger &result) {
    bigAdd(n0, -n1, result);
}

BigInteger BigInteger::operator+=(const BigInteger &n1) {
    BigInteger temp(*this + n1);
    return *this = temp;
}

BigInteger BigInteger::operator++() {
    return *this = *this + 1;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp(*this);
    *this += 1;
    return temp;
}

BigInteger BigInteger::operator--() {
    return *this = *this - 1;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp(*this);
    *this -= 1;
    return temp;
}

BigInteger operator-(const BigInteger &n1, const BigInteger &n2) {
    BigInteger ret(0);
    bigSub(n1, n2, ret);
    return ret;
}

BigInteger BigInteger::operator-=(const BigInteger &n1) {
    BigInteger temp(*this - n1);
    return *this = temp;
}

BigInteger smallMul(const BigInteger &n0, const BigInteger &n1) {
    //n0.size or n1.size must be 1
    if (n0.size == 1) {
        digit m = n0.num[0];
        if (n0.sign)
            return -tinyMul1(n1, m);
        return tinyMul1(n1, m);
    }
    digit m = n1.num[0];
    if (n1.sign)
        return -tinyMul1(n0, m);
    return tinyMul1(n0, m);
}

BigInteger largeMul(const BigInteger &n0, const BigInteger &n1) {
    //both numbers must be big enough
    BigInteger result(0);
    if(n0.size >= n1.size) {
        KaratsubaMul(n0, n1, result);
        return result;
    }
    KaratsubaMul(n1, n0, result);
    return result;
}

BigInteger operator*(const BigInteger &n0, const BigInteger &n1) {
    if (n0.size == 0 || n1.size == 0)
        return 0;
    long n0Size = n0.size, n1Size = n1.size;
    if (n0Size == 1 || n1Size == 1) //one of numbers is small
        return smallMul(n0, n1);
    //both numbers are big enough
    if((n0Size >= BigInteger::KaratsubaMin || n1Size >= BigInteger::KaratsubaMin) && 
        (n0Size >= BigInteger::KaratsubaMin/2 || n1Size >= BigInteger::KaratsubaMin/2)) {
        return largeMul(n0, n1);
    }
    BigInteger result(0);
    uLong length = n1Size + n0Size, temp, carry;
    result.num.resize(length);
    result.size = length;
    digit *rslt = &result.num[0];
    const digit *s0 = &n0.num[0], *s1 = &n1.num[0];
    for (uLong i = 0; i < length; i++)
        rslt[i] = 0;
    long j = 0;
    for (long i = 0; i < n0Size; i++) {
        carry = 0;
        for (j = 0; j < n1Size; j++) {
            temp = (uLong)s0[i] * s1[j] + rslt[i+j] + carry;
            carry = temp >> BigInteger::BaseLog;
            rslt[i+j] = temp & BigInteger::BaseMod;//temp%BASE;
        }
        rslt[i+j] = carry;
    }
    result.deleteLeadingZeroes();
    result.sign = n0.sign ^ n1.sign;
    return result;
}

BigInteger BigInteger::operator*=(const BigInteger &n1) {
    BigInteger temp(*this*n1);
    return *this = temp;
}

void bigDivide(const BigInteger &n0, const BigInteger &n1, BigInteger &Q, BigInteger &R) {//n0 < n1
    if (absCompare(n1, n0)) {//if (abs(n0) < abs(n1))
        Q = 0;
        R = n0;
        return;
    }
    BigInteger U(abs(n0)), B1(abs(n1));
    long n = B1.size, l0 = U.size;
    digit scale = BigInteger::BASE / (n1.num[n1.size - 1] + 1), r;
    long long qGuess, borrow, carry;
    if (scale > 1) {
        tinyMul(U, scale, U);//U *= scale;
        tinyMul(B1, scale, B1);//B1 *= scale;
    }
    if(l0 == U.size) {
        U.size++;
        U.num.push_back(0);
    }
    long m = l0 - n + 1;
    Q.num.resize(m);
    Q.size = m;
    digit *ret = &Q.num[0];
    for (long i = 0; i < m; i++)
        ret[i] = 0;
    digit *b = &B1.num[B1.size - 1], *u = &U.num[U.size - 1], *q = &Q.num[Q.size - 1];
    long long temp1, temp2;
    long i;
    for(long j = 0; j > -m; j--) {
        qGuess = (((uLong)u[j] << BigInteger::BaseLog) + u[j - 1]) / b[0];
        r = (((uLong)u[j] << BigInteger::BaseLog) + u[j - 1]) % b[0];
        while ( r < BigInteger::BASE) {
            temp2 = b[-1]*qGuess;
            temp1 = ((uLong)r << BigInteger::BaseLog) + u[j - 2];
            if ( temp2 > temp1 || qGuess == BigInteger::BASE ) {
                qGuess--;
                r += b[0];
            } else break;
        }
        digit *shift = u + j;
        carry = borrow = 0;
        for (i = n - 1; i >= 0; i--) {
            temp1 = b[-i] * qGuess + carry;
            carry = temp1 >> BigInteger::BaseLog;
            temp1 &= BigInteger::BaseMod;
            temp2 = shift[-i - 1] - temp1 + borrow;
            shift[-i - 1] = temp2 & BigInteger::BaseMod;
            borrow = temp2 >> BigInteger::BaseLog;
        }
        temp2 = shift[-i - 1] - carry + borrow;
        shift[-i - 1] = temp2 & BigInteger::BaseMod;
        borrow = temp2 >> BigInteger::BaseLog;
        q[j] += qGuess + borrow;
        if (borrow != 0) {
            carry = 0;
            for (i = n - 1; i >= 0; i--) {
                temp2 = shift[-i - 1] + b[-i] + carry;
                shift[-i - 1] = temp2 & BigInteger::BaseMod;
                carry = temp2 >> BigInteger::BaseLog;
            }
            shift[-i - 1] = (shift[-i - 1] + carry) & BigInteger::BaseMod;
        }
    }
    Q.deleteLeadingZeroes();
    R = U;
    if (scale > 1)
        tinyDivide(U, R, scale, r);
    R.deleteLeadingZeroes();
    Q.sign = n1.sign^n0.sign;
    return;
}

void tinyDivide(const BigInteger &A, BigInteger &Q, digit s, digit &R) {
    if (A.num != Q.num) {
        Q.num.clear();
        Q.num.resize(A.size, 0);
        Q.size = A.size;
    }
    const digit *a = &A.num[0];
    digit *q = &Q.num[0];
    R = 0;
    for(long i = A.size - 1; i >= 0; i--) {
        uLong temp = a[i] + ((uLong)R << BigInteger::BaseLog);
        q[i] = temp/s;
        R = temp - q[i]*s;
    }
    Q.deleteLeadingZeroes();
    Q.sign = A.sign;
}

BigInteger operator/(const BigInteger &n0, const BigInteger &n1) {
    if (n1 == 0)
        throw BigIntegerException("Division by zero", __FILE__, __LINE__);
    BigInteger R(0), Q(0);
    if ( n1.size == 1) {
          digit r;
          tinyDivide(n0, Q, n1.num[0], r);
          Q.sign = n0.sign ^ n1.sign;
          return Q;
    }
    bigDivide(n0, n1, Q, R);
    return Q;
}

BigInteger BigInteger::operator/=(const BigInteger &n1) {
    BigInteger temp(*this/n1);
    return *this = temp;
}

BigInteger operator%(const BigInteger &n0, const BigInteger &n1) {
    if (n1 == 0)
        throw BigIntegerException("Modulo by zero", __FILE__, __LINE__);
    BigInteger R(0), Q(0);
    if ( n1.size == 1) {
          digit r;
          tinyDivide(n0, Q, n1.num[0], r);
          return r;
    }
    bigDivide(n0, n1, Q, R);
    return R;
}

BigInteger BigInteger::operator%=(const BigInteger &n1) {
    BigInteger temp(*this%n1);
    return *this = temp;
}

BigInteger pow(BigInteger n1, BigInteger n2) {
    BigInteger b(1);
    if (n2.sign)
        throw BigIntegerException("Negative power", __FILE__, __LINE__);
    while (n2 != 0)
        if (n2%2 == 0) {
            n2 = n2 / 2;
            n1 = n1*n1;
        } else {
            n2--;
            b = b*n1;
        }
    return b;
}

void tinyMul(const BigInteger &A, digit B, BigInteger &C) {
    if (&A.num != &C.num) {
        C.num.clear();
        C.num.resize(A.size, 0);
        C.size = A.size;
    }
    digit *c = &C.num[0];
    const digit *a = &A.num[0];
    uLong temp = 0, carry = 0;
    for(long i = 0; i < A.size; i++) {
        temp = (uLong)a[i]*B + carry;
        carry = temp >> BigInteger::BaseLog;
        c[i] = temp & BigInteger::BaseMod;//temp - carry*BigInteger::BASE;
    }
    if(carry) {
         C.size++;
         C.num.push_back(carry);
    }
    C.sign = A.sign;
}

BigInteger tinyMul1(const BigInteger &A, digit B) {
    BigInteger C(A);
    tinyMul(A, B, C);
    return C;
}

void KaratsubaMul(const BigInteger &A, const BigInteger &B, BigInteger &C) {//A.size >= B.size
    BigInteger a1(0), a2(0), b1(0), b2(0);                                 //Karatsuba algorithm
    C.size = A.size + B.size;
    C.num.resize(C.size);
    digit *c = &C.num[0];
    for (long i = 0; i < C.size; i++)
        c[i] = 0;
    a1.size = A.size/2;
    a2.size = (A.size + 1)/2;
    a1.num.resize(a1.size);
    memcpy(&a1.num[0], &A.num[A.size - a1.size], a1.size*BigInteger::sizeOfDigit);
    a2.num.resize(a2.size);
    memcpy(&a2.num[0], &A.num[0], a2.size*BigInteger::sizeOfDigit);
    if (B.size <= a2.size) {
        b2.size = B.size;
        b2.num.resize(b2.size);
        memcpy(&b2.num[0], &B.num[0], b2.size*BigInteger::sizeOfDigit);
    } else {
        b2.size = a2.size;
        b1.size = B.size - b2.size;
        b1.num.resize(b1.size);
        memcpy(&b1.num[0], &B.num[B.size - b1.size], b1.size*BigInteger::sizeOfDigit);
        b2.num.resize(b2.size);
        memcpy(&b2.num[0], &B.num[0], b2.size*BigInteger::sizeOfDigit);
    }
    long a2Size = a2.size;
    a1.deleteLeadingZeroes();
    a2.deleteLeadingZeroes();
    b1.deleteLeadingZeroes();
    b2.deleteLeadingZeroes();
    BigInteger temp2 = a2*b2, temp3 = a1*b1, temp1 = (a1 + a2)*(b1 + b2) - temp2 - temp3;
    memcpy(c, &temp2.num[0], temp2.size*BigInteger::sizeOfDigit);
    memcpy(c + 2*a2Size, &temp3.num[0], temp3.size*BigInteger::sizeOfDigit);
    digit *ptr = c + a2Size, *tmp = &temp1.num[0], *cEnd = c + C.size;
    uLong carry = 0;
    for(long i = 0; i < temp1.size; i++) {
        carry += tmp[i] + *ptr;
        *ptr++ = carry & BigInteger::BaseMod;
        carry >>= BigInteger::BaseLog;
    }
    if (carry) {
        while (ptr < cEnd) {
            carry += *ptr;
            *ptr++ = carry & BigInteger::BaseMod;
            carry >>= BigInteger::BaseLog;
        }
        if (carry) {
            C.size++;
            C.num.push_back(carry);
        }
    }
    C.deleteLeadingZeroes();
    C.sign = A.sign ^ B.sign;
}

void BigInteger::deleteLeadingZeroes() {
    while(this->num[size - 1] == 0 && size >= 1) {///not sure but may be true
        size--;
        num.pop_back();
    }
    if (this->size == 0)
        this->sign = 0;
    return;
}

uLong min(uLong a, uLong b) {
    return (a < b) ? a : b;
}

void BigInteger::fromDecimal(uLong* &A, long &size, digit &R) {//digit *a = A.num;
    R = 0;
    for(long i = 0; i < size; i++) {
        uLong temp = R*Base3, temp2 = temp, tmp = 0;
        if (R != 0 && temp / R != Base3)
            tmp = 1;
        temp = temp2 + A[i];
        if (temp < min(A[i], temp2))
            tmp++;
        A[i] = (tmp << 33) + (temp >> BigInteger::BaseLog);
        R = temp & BigInteger::BaseMod;
    }
    long i = 0;
    while (A[i] == 0 && size > 0) {
        i++;
        size--;
    }
    if (i > 0) {
        uLong *tmp = new uLong[size];
        memcpy(tmp, A + i, size*BigInteger::sizeOfuLong);
        delete [] A;
        A = tmp;
    }
}

BigInteger gcd(const BigInteger &a, const BigInteger &b) {
    //The greatest common divisor
    BigInteger m(abs(a)), n(abs(b)), temp(0);
    while (m != 0) {
        temp = n;
        n = m;
        m = temp%m;
    }
    return n;
}

BigInteger lcm(const BigInteger &a, const BigInteger &b) {
    //Least common multiple
    return abs(a*b)/gcd(a, b);
}

BigInteger fib(const BigInteger &n) {
    //n-th Fibonacci number
    if (n.sign)
        throw BigIntegerException("Negative Fibonacci number", __FILE__, __LINE__);
    if (n == 1 || n == 2)
        return 1;
    BigInteger n1(0), n2(1), count(n), answer(0);
    while (count-- >= 2) {
        answer = n1;
        n1 = n2;
        answer += n1;
        n2 = answer;
    }
    return answer;
}

BigInteger fact(const BigInteger&n) {
    //factorial(n) = n!
    if (n.size > 1)
        throw BigIntegerException("Too large factorial number", __FILE__, __LINE__);
    digit value = n.toUint();
    BigInteger result(1);
    for (digit i = 2; i <= value; i++)
        result *= i;
    return result;
}

unsigned int BigInteger::toUint() const {
    if (size == 0 || size > 2)//ignore these numbers - they are too big
        return 0;
    if (size == 1)
        return num[0];
    else
        return num[1]*BigInteger::BASE + num[0];
}

BigInteger log10(BigInteger n) {
    BigInteger i = 0;
    while (n >= 10) {
        i++;
        n /= 10;
    }
    return i;
}

std::string BigInteger::toString(int n, int mode) {
    if (size == 0)
        return "0";
    std::ostringstream oss;
    if (n && abs(*this) > pow(10, n)) {
        BigInteger lg10(log10(abs (*this))), A(*this/pow(10, lg10 - 5)), B(A/100000);
        A = A%100000;
        oss << B << "." << A << "e+" << lg10;
    }
    else if (mode || *this < 10000000)
        oss << *this;
    else {
        BigInteger temp(*this);
        std::vector<int> v;
        while (temp != 0) {
            v.push_back((temp%1000).toUint());
            temp = temp/1000;
        }
        oss << v.back();
        v.pop_back();
        while (!v.empty()) {
            oss << '\'';
            int digitN = v.back(), power = 100;
            v.pop_back();
            for (int i = 0; i < 3; i++) {
                oss << digitN/power;
                digitN %= power;
                power /= 10;
            }
        }
    }
    return oss.str();
}

BigInteger operator>>(const BigInteger &n1, const BigInteger &n2) {
    if (n2 == 0)
        return n1;
    if (n2 > 0)
        return n1/pow(2, abs(n2));
    return n1*pow(2, abs(n2));
}

BigInteger operator<<(const BigInteger &n1, const BigInteger &n2) {
    if (n2 == 0)
        return n1;
    if (n2 > 0)
        return n1*pow(2, abs(n2));
    return n1/pow(2, abs(n2));
}
