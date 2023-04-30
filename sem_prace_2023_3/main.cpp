#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include <filesystem>
#include "my_big_int.h"

/** Size of the key in bits */
constexpr int KEY_SIZE = 250;
/** Directory where the data is stored */
constexpr std::string_view DATA_SOURCE = "validation/";
/** Directory where the output is stored */
constexpr std::string_view OUTPUT_DIR = "out/";
/** Directory where the decoded data is stored */
constexpr std::string_view DECODED_DIR = "decoded/";
/** 0, 1 and 2 in MPInt */
MPInt<int32_t, UNLIMITED> ZERO("0"),
                          ONE("1"),
                          TWO("2");
/** Vector of powers of 2 */
std::vector<MPInt<int32_t, UNLIMITED>> TWO_POWS;

/**
 * Initializes the vector of powers of 2
 * @param size Size of the vector (max power of 2)
 */
void init_two_pows(int size) {
    TWO_POWS.push_back(ONE);
    for (int i = 1; i < size; i++)
        TWO_POWS.push_back(TWO_POWS[i - 1] * TWO);
}

/**
 * Converts a vector of bits to a decimal number (MPInt)
 * @param bits Vector of bits
 * @return MPInt representation of the bits
 */
MPInt<int32_t, UNLIMITED> bits_to_dec(const std::vector<bool> &bits) {
    MPInt<int32_t, UNLIMITED> res("0");
    auto bits_reverse{bits};
    std::reverse(bits_reverse.begin(), bits_reverse.end());
    for (int i = 0; i < bits_reverse.size(); i++) {
        if (bits_reverse[i])
            res += TWO_POWS[i];
    }
    return res;
}

/**
 * Converts a decimal number (MPInt) to a vector of bits
 * @param dec MPInt representation of the bits
 * @return Vector of bits
 */
std::vector<bool> dec_to_bits(MPInt<int32_t, UNLIMITED> dec) {
    std::vector<bool> res;
    while (dec > ZERO) {
        res.push_back(dec % TWO == ONE);
        dec /= TWO;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

/**
 * Generates a vector of random bits
 * @param size Size of the vector
 * @return Vector of random bits
 */
std::vector<bool> gen_rng_bits(int size) {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<bool> res;
    for (int i = 0; i < size; ++i)
        res.push_back(dist(gen));
    return res;
}

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
 * Generates a private key (it is sure that the sequence is super increasing)
 * @return Private key
 */
std::vector<std::vector<bool>> gen_private_key() {
    std::vector<std::vector<bool>> res(KEY_SIZE);
    MPInt<int32_t, UNLIMITED> sum("0");
    for (int i = 0; i < KEY_SIZE; i++) {
        // Generate a random number between 0 and 2^100 and add it to the sum of the previous numbers
        // This ensures that the sequence is super increasing (thus the + 1)
        MPInt<int32_t, UNLIMITED> next = bits_to_dec(gen_rng_bits(100)) + sum + ONE;
        sum += next;
        res[i] = dec_to_bits(next);
    }
    return res;
}

/**
 * Generates q parameter, which has to be bigger than the sum of the private key
 * @param private_key_dec Private key in decimal form
 * @return q parameter
 */
std::vector<bool> gen_q(const std::vector<MPInt<int32_t, UNLIMITED>> &private_key_dec) {
    MPInt<int32_t, UNLIMITED> cum_sum("0");
    for (auto &key : private_key_dec)
        cum_sum += key;
    MPInt<int32_t, UNLIMITED> q = bits_to_dec(gen_rng_bits(350)) + cum_sum + ONE;
    return dec_to_bits(q);
}

/**
 * Generates a public key from a private key
 * @param private_key_dec Private key in decimal form
 * @param p P parameter
 * @param q Q parameter
 * @return Public key
 */
std::vector<std::vector<bool>> gen_public_key(const std::vector<MPInt<int32_t, UNLIMITED>> &private_key_dec, int p, const std::vector<bool> &q) {
    std::vector<std::vector<bool>> res(KEY_SIZE);
    MPInt<int32_t, UNLIMITED> q_dec = bits_to_dec(q);
    MPInt<int32_t, UNLIMITED> p_dec(std::to_string(p));
    for (int i = 0; i < KEY_SIZE; i++) {
        auto private_key_dec_i = private_key_dec[i];
        auto public_key_dec = (private_key_dec_i * p_dec) % q_dec;
        res[i] = dec_to_bits(public_key_dec);
    }
    return res;
}

/**
 * Non recursive implementation of the extended euclidean algorithm
 * @param a Number to find the multiplicative inverse of
 * @param b Modulo number (the number to find the multiplicative inverse of a modulo b)
 * @return Multiplicative inverse of a modulo b
 */
MPInt<int32_t, UNLIMITED> find_multiplicative_inverse(MPInt<int32_t, UNLIMITED> a, MPInt<int32_t, UNLIMITED> b) {
    MPInt<int32_t, UNLIMITED> x1("0"), y1("1"), x("1"), y("0");
    while (a != ZERO) {
        MPInt<int32_t, UNLIMITED> q = b / a;
        MPInt<int32_t, UNLIMITED> t = a;
        a = b % a;
        b = t;
        t = x;
        x = x1 - q * x;
        x1 = t;
        t = y;
        y = y1 - q * y;
        y1 = t;
    }
    return x1;
}

/**
 * Generates p inverse parameter
 * @param p P parameter
 * @param q Q parameter
 * @return p inverse parameter
 */
MPInt<int32_t, UNLIMITED> gen_p_inverse(int p, const std::vector<bool>& q) {
    MPInt<int32_t, UNLIMITED> p_dec(std::to_string(p));
    MPInt<int32_t, UNLIMITED> q_dec = bits_to_dec(q);
    return q_dec + find_multiplicative_inverse(p_dec, q_dec);
}

/**
 * Encodes bytes into a vector of integers
 * @param input_bytes Input bytes
 * @param public_key_dec Public key in decimal form
 * @param padding Padding of the input bits (number is modified)
 * @return Vector of encoded integers (in decimal form) (MPInt)
 */
std::vector<MPInt<int32_t, UNLIMITED>> encode(const std::vector<unsigned char> &input_bytes, const std::vector<MPInt<int32_t, UNLIMITED>> &public_key_dec, int *padding) {
    std::vector<bool> input_bits;
    for (auto &byte : input_bytes) {
        std::vector<bool> byte_bits;
        for (int i = 0; i < 8; i++)
            byte_bits.push_back((byte >> i) & 1);
        std::reverse(byte_bits.begin(), byte_bits.end());
        input_bits.insert(input_bits.end(), byte_bits.begin(), byte_bits.end());
    }

    while (input_bits.size() % KEY_SIZE) {
        input_bits.push_back(false);
        (*padding)++;
    }

    std::vector<MPInt<int32_t, UNLIMITED>> res;
    for (int i = 0; i < input_bits.size(); i += KEY_SIZE) {
        MPInt<int32_t, UNLIMITED> sum("0");
        for (int j = 0; j < KEY_SIZE; j++) {
            if (input_bits[i + j])
                sum += public_key_dec[j];
        }
        res.push_back(sum);
    }

    return res;
}

/**
 * Decodes a vector of integers into bytes
 * @param input Input vector of integers (in decimal form) (MPInt)
 * @param private_key_dec Private key in decimal form
 * @param p_inverse P inverse parameter
 * @param q Q parameter
 * @param padding Padding of the input bits (number was modified while encoding)
 * @return Decoded bytes
 */
std::vector<unsigned char> decode(std::vector<MPInt<int32_t, UNLIMITED>> &input, const std::vector<MPInt<int32_t, UNLIMITED>> &private_key_dec, MPInt<int32_t, UNLIMITED> &p_inverse, const std::vector<bool> &q, int padding) {
    auto q_dec = bits_to_dec(q);
    std::vector<bool> res_bits;
    std::vector<unsigned char> res;

    for (auto &input_dec : input) {
        auto temp = (input_dec * p_inverse) % q_dec;
        std::vector<bool> bits(KEY_SIZE);
        for (int i = KEY_SIZE - 1; i >= 0; i--) {
            if (temp >= private_key_dec[i]) {
                bits[i] = true;
                temp -= private_key_dec[i];
            }
        }
        res_bits.insert(res_bits.end(), bits.begin(), bits.end());
    }

    while (padding--)
        res_bits.pop_back();

    for (int i = 0; i < res_bits.size(); i += 8) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; j++)
            byte |= res_bits[i + j] << (7 - j);
        res.push_back(byte);
    }

    return res;
}

/**
 * Main function
 */
int main() {
    init_two_pows(400); // Initialize vector of powers of 2
    std::cout << "Generating keys..." << std::endl;
    auto private_key = gen_private_key();
    std::vector<MPInt<int32_t, UNLIMITED>> private_key_dec;
    for (auto &key : private_key)
        private_key_dec.push_back(bits_to_dec(key));
    auto q = gen_q(private_key_dec);
    auto p = gen_prime(); // This ensures that p and q don't have common factors (except 1)
    auto public_key = gen_public_key(private_key_dec, p, q);
    std::vector<MPInt<int32_t, UNLIMITED>> public_key_dec;
    for (auto &key : public_key)
        public_key_dec.push_back(bits_to_dec(key));
    auto p_inverse = gen_p_inverse(p, q); // This always exists, because p is prime and q is whatever really

    std::ofstream p_file("p.txt");
    std::ofstream q_file("q.txt");
    std::ofstream private_key_file("private_key.txt");
    std::ofstream public_key_file("public_key.txt");

    p_file << p;
    q_file << bits_to_dec(q);
    for (int i = 0; i < KEY_SIZE; i++) {
        private_key_file << private_key_dec[i];
        public_key_file << public_key_dec[i];
        if (i != KEY_SIZE - 1) {
            private_key_file << ",";
            public_key_file << ",";
        }
    }

    p_file.close();
    q_file.close();
    private_key_file.close();
    public_key_file.close();

    if (std::filesystem::exists(DATA_SOURCE)) {
        std::vector<std::string> files;
        for (const auto &entry: std::filesystem::directory_iterator(DATA_SOURCE))
            files.push_back(entry.path().string());

        for (const auto &filepath: files) {
            auto basename_filepath = filepath.substr(filepath.find_last_of("/\\") + 1);
            auto filename = basename_filepath.substr(0, basename_filepath.find_last_of('.'));
            auto extension = basename_filepath.substr(basename_filepath.find_last_of('.') + 1);

            auto input_bytes = std::vector<unsigned char>();
            std::ifstream input(filepath, std::ios::binary);
            std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(),
                      std::back_inserter(input_bytes));

            if (input_bytes.empty()) {
                std::cout << "File " << filepath << " is empty" << std::endl;
                continue;
            }

            int padding = 0;
            auto hex_out_filename = std::string(OUTPUT_DIR).append(filename).append("_decrypted_hexoutput.txt");
            auto encoded_filepath = std::string(OUTPUT_DIR).append(filename).append("_encrypted_output.txt");
            auto decoded_filepath = std::string(DECODED_DIR).append(filename).append(".").append(extension);

            // Encoding
            std::cout << "Encrypting file " << filepath << std::endl;
            auto encoded_decimals = encode(input_bytes, public_key_dec, &padding);
            if (!encoded_decimals.empty()) {
                std::ofstream output(encoded_filepath);
                for (auto &dec : encoded_decimals)
                    output << dec << std::endl;
            }

            // Decoding
            std::cout << "Decrypting file " << encoded_filepath << std::endl;
            auto decoded_bytes = decode(encoded_decimals, private_key_dec, p_inverse, q, padding);
            if (!decoded_bytes.empty()) {
                std::ofstream decoded(decoded_filepath, std::ios::binary);
                decoded.write((char *) decoded_bytes.data(), static_cast<int>(decoded_bytes.size()));

                std::ofstream output_hex(hex_out_filename, std::ios::binary);
                for (int i = 0; i < 100 && i < decoded_bytes.size(); i++)
                    output_hex << std::hex << std::setfill('0') << std::setw(2)
                               << static_cast<int>(decoded_bytes.at(i));
            }
        }
    }

    return EXIT_SUCCESS;
}
