#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <fstream>
#include "sha256.h"
#include "biginteger.h"

/** String to hash and later sign */
constexpr std::string_view A20B0279P = "A20B0279P";

/**
 * Checks if a number is prime and bigger than 2
 * @param n Number to check
 * @return True if the number is prime and bigger than 2, false otherwise
 */
bool is_prime_and_bigger_than_two(int n) {
    if (n <= 2)
        return false;
    if (n % 2 == 0 || n % 3 == 0)
        return false;
    for (int i = 5; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

/**
 * Generates a random prime number between 3 and upper_limit
 * @param upper_limit Upper limit of the random number
 * @return Random prime number between 3 and upper_limit
 */
int gen_prime(int upper_limit=INT32_MAX) {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, upper_limit);
    int res = dist(gen);
    while (!is_prime_and_bigger_than_two(res))
        res = dist(gen);
    return res;
}

/**
 * Generates a random number between 0 and p - 1
 * @param p Upper limit of the random number
 * @return Random number between 0 and p - 1
 */
int gen_random_smaller_than_p(int p) {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, p - 1);
    return dist(gen);
}

/**
 * Non recursive implementation of the extended euclidean algorithm
 * @param a Number to find the multiplicative inverse of
 * @param b Modulo number (the number to find the multiplicative inverse of a modulo b)
 * @return Multiplicative inverse of a modulo b
 */
int find_multiplicative_inverse(int a, int b) {
    int t = 0, new_t = 1, r = b, new_r = a;
    while (new_r != 0) {
        int quotient = r / new_r;
        int temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;
        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }
    t += b;
    return t;
}

/**
 * Calculates the modular exponentiation of a number
 * @param base Base number
 * @param exponent Exponent
 * @param modulus Modulus
 * @return Modular exponentiation of base^exponent mod modulus
 */
BigInteger mod_pow(BigInteger base, BigInteger exponent, BigInteger modulus){
    BigInteger result = 1;
    while (exponent > 0){
        if (exponent % 2 == 1)
            result = (result * base) % modulus;
        exponent = exponent >> 1;
        base = (base * base) % modulus;
    }
    return result;
}

/**
 * Parses a hash string to a vector of bytes
 * @param hash Hash string
 * @return Vector of bytes
 */
std::vector<int> get_hash_bytes(const std::string &hash) {
    std::vector<int> res;
    for (int i = 0; i < hash.size(); i += 2) {
        std::string hex = hash.substr(i, 2);
        res.push_back(std::stoi(hex, nullptr, 16));
    }
    return res;
}

/**
 * Sums the bytes of a hash into a single decimal number
 * @param hex_bytes Vector of bytes
 * @return Decimal number
 */
BigInteger hex_to_dec(const std::vector<int> &hex_bytes) {
    BigInteger res = 0;
    for (int i = 0; i < hex_bytes.size(); i++)
        res += BigInteger(hex_bytes[i]) * pow(BigInteger(256), BigInteger(hex_bytes.size() - i - 1));
    return res;
}

/**
 * Generates a random prime number p
 * @return Random prime number p
 */
int gen_p() {
    return gen_prime();
}

/**
 * Generates a random number g smaller than p
 * @param p Prime number
 * @return Random number g smaller than p
 */
int gen_g(int p) {
    return gen_random_smaller_than_p(p);
}

/**
 * Generates a random number x smaller than p
 * @param p Prime number
 * @return Random number x smaller than p
 */
int gen_x(int p) {
    return gen_random_smaller_than_p(p);
}

/**
 * Generates a number y as g^x mod p
 * @param g g
 * @param x x
 * @param p p
 * @return Number y as g^x mod p
 */
int gen_y(int g, int x, int p) {
    BigInteger big_x = BigInteger(x);
    BigInteger big_g = BigInteger(g);
    BigInteger big_y = mod_pow(big_g, big_x, p);
    int y = static_cast<int>(big_y.toUint());
    return y % p;
}

/**
 * Signs a message using the ElGamal signature scheme
 * @param message Message to sign
 * @param p p
 * @param g g
 * @param x x
 * @return Pair of numbers (a, b) as the signature of the message
 */
std::pair<BigInteger, BigInteger> sign(BigInteger message, int p, int g, int x) {
    int k = gen_prime(p - 1);
    std::ofstream k_file("k.txt");
    std::cout << "k = " << k << std::endl;
    k_file << k;
    k_file.close();

    BigInteger a = mod_pow(g, k, p);
    std::cout << "a = " << a << std::endl;

    int k_inv = find_multiplicative_inverse(k, p - 1);
    auto temp = (message - x * a);
    if (temp < 0) {
        temp %= (p - 1);
        temp = (p - 1) - temp;
    }
    else {
        temp %= (p - 1);
    }
    BigInteger b = (k_inv * temp) % (p - 1);
    std::cout << "b = " << b << std::endl;

    return std::make_pair(a, b);
}

/**
 * Verifies the signature of a message using the ElGamal signature scheme
 * @param message Message
 * @param signature Signature of the message to verify
 * @param g g
 * @param p p
 * @param y y
 * @return True if the signature is valid, false otherwise
 */
bool verify(BigInteger message, std::pair<BigInteger, BigInteger> &signature, int g, int p, int y) {
    BigInteger a = signature.first;
    BigInteger b = signature.second;

    BigInteger left = mod_pow(y, a, p) * mod_pow(a, b, p) % p;
    std::cout << "y^a * a^b mod p = " << left << std::endl;

    BigInteger right = mod_pow(g, message, p);
    std::cout << "g^message mod p = " << right << std::endl;

    return left == right;
}

/**
 * Main function
 * @return exit code
 */
int main() {
    auto hash = sha256(A20B0279P.data());
    std::cout << A20B0279P << " -> " << hash << std::endl;
    auto hash_bytes = get_hash_bytes(hash);
    auto hash_num = hex_to_dec(hash_bytes);
    std::cout << "Hash as decimal number: " << hash_num << std::endl;

    std::cout << std::endl << "Generating parameters:" << std::endl;
    int p = gen_p();
    std::ofstream p_file("p.txt");
    std::cout << "p = " << p << std::endl;
    p_file << p;
    p_file.close();

    int g = gen_g(p);
    std::ofstream g_file("g.txt");
    std::cout << "g = " << g << std::endl;
    g_file << g;
    g_file.close();

    int x = gen_x(p);
    std::ofstream x_file("x.txt");
    std::cout << "x = " << x << std::endl;
    x_file << x;
    x_file.close();

    int y = gen_y(g, x, p);
    std::ofstream y_file("y.txt");
    std::cout << "y = " << y << std::endl;
    y_file << y;
    y_file.close();

    std::cout << std::endl << "Signing:" << std::endl;
    auto signature = sign(hash_num, p, g, x);
    std::ofstream signature_file("signature.txt");
    std::cout << "Signature: " << signature.first << "," << signature.second << std::endl;
    signature_file << signature.first << "," << signature.second;
    signature_file.close();

    std::cout << std::endl << "Verifying:" << std::endl;
    auto verification = verify(hash_num, signature, g, p, y);
    std::cout << "Verification result: " << (verification ? "OK" : "FAILED") << std::endl;

    return EXIT_SUCCESS;
}
