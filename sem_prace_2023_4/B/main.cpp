#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "biginteger.h"

// https://github.com/qiray/BigInteger

/** Diffie-Hellman source file */
constexpr std::string_view SOURCE_FILE = "diffie_hellman_keys.csv";
/** Time limit in seconds */
constexpr int TIME_LIMIT_IN_SECONDS = 900;

/** Mutex for thread synchronization */
std::mutex mtx;
/** Flag for thread synchronization */
bool completed = false;

/**
 * Brute force attack on Diffie-Hellman key exchange
 * @param p Prime number
 * @param g Generator
 * @param g_pow_pkey_mod_p g^pkey mod p
 * @return Private key
 */
BigInteger dh_bf_attack(const BigInteger& p, const BigInteger& g, const BigInteger& g_pow_pkey_mod_p) {
    BigInteger x = 0;
    BigInteger g_pow_x_mod_p = 1;
    while (g_pow_x_mod_p != g_pow_pkey_mod_p) {
        x++;
        g_pow_x_mod_p = (g_pow_x_mod_p * g) % p;
    }
    return x;
}

/**
 * Kill the program after 15 minutes
 */
void kill_in_15_mins() {
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= TIME_LIMIT_IN_SECONDS)
            break;
        mtx.lock();
        if (completed) {
            mtx.unlock();
            return;
        }
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "Time limit exceeded, exiting..." << std::endl;
    exit(EXIT_FAILURE);
}

/**
 * Main function
 * @return Exit code
 */
int main() {
    std::vector<BigInteger> p;
    std::vector<BigInteger> g;
    std::vector<BigInteger> g_pow_pkey_mod_p;

    std::cout << "Reading file..." << std::endl;
    std::ifstream file((std::string(SOURCE_FILE)));
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        auto separator_pos = line.find(';');
        line = line.substr(separator_pos + 1);
        separator_pos = line.find(';');
        line = line.substr(separator_pos + 1);
        separator_pos = line.find(';');
        p.emplace_back((line.substr(0, separator_pos)));
        line = line.substr(separator_pos + 1);
        separator_pos = line.find(';');
        g.emplace_back((line.substr(0, separator_pos)));
        line = line.substr(separator_pos + 1);
        g_pow_pkey_mod_p.emplace_back((line));
    }

    std::vector<BigInteger> results;
    std::vector<double> times;
    std::thread killer(kill_in_15_mins);

    std::cout << "Starting attack..." << std::endl;
    for (int i = 0; i < g_pow_pkey_mod_p.size(); i++) {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << std::endl << "Attacking (" << g[i] << "^x) % " << p[i] << " = " << g_pow_pkey_mod_p[i] << std::endl;
        results.push_back(dh_bf_attack(p[i], g[i], g_pow_pkey_mod_p[i]));
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0);
        std::cout << "Found x = " << results.back() << std::endl;
        std::cout << "Time taken: " << times.back() << "s" << std::endl;
    }

    std::ofstream output("alices_private_keys_A20B0279P.csv");
    for (int i = 0; i < results.size(); i++) {
        output << results[i] << ";" << times[i] << std::endl;
    }

    mtx.lock();
    completed = true;
    mtx.unlock();

    killer.join();
    return EXIT_SUCCESS;
}
