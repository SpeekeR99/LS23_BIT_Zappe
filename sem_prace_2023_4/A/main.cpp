#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <thread>
#include "sha256.h"

// https://github.com/hlilje/sha-256

/** Username */
constexpr std::string_view MY_USERNAME = "A20B0279P";
/** Password database file */
constexpr std::string_view PASSWORD_FILE = "password_database.csv";
/** Password dictionary file */
constexpr std::string_view PASSWORD_DICT = "password_dict.txt";
/** Possible characters for brute force attack - format numbers then letters */
constexpr std::string_view POSSIBLE_NUMS_CHARS = "0123456789abcdefghijklmnopqrstuvwxyz";
/** Possible characters for brute force attack - format letters then numbers */
constexpr std::string_view POSSIBLE_CHARS_NUMS = "abcdefghijklmnopqrstuvwxyz0123456789";
/** Possible characters for brute force attack - format letters only */
constexpr std::string_view POSSIBLE_CHARS = "abcdefghijklmnopqrstuvwxyz";
/** Possible characters for brute force attack - format numbers only */
constexpr std::string_view POSSIBLE_NUMS = "0123456789";
/** Maximum time limit for brute force attack in seconds */
constexpr int TIME_LIMIT_IN_SECONDS = 180;

/** Mutex for thread synchronization */
std::mutex mtx;
/** Flag for thread synchronization */
bool found = false;
/** Result of brute force attack */
std::string bf_found_result;

/**
 * Reads the password dictionary file and returns a vector of strings
 * @return
 */
std::vector<std::string> get_pass_dict() {
    std::vector<std::string> pass_dict;

    std::ifstream file((std::string(PASSWORD_DICT)));
    std::string line;
    while (std::getline(file, line))
        pass_dict.push_back(line + "\n");

    return pass_dict;
}

/**
 * Reads the password database file and returns a vector of strings
 * @param username Username to filter by
 * @return Vector of strings containing the hashes
 */
std::vector<std::string> get_hashes(const std::string& username = std::string(MY_USERNAME)) {
    std::vector<std::string> hashes;

    std::ifstream file((std::string(PASSWORD_FILE)));
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(username) != std::string::npos)
            hashes.push_back(line.substr(line.find(',') + 1));
    }

    return hashes;
}

/**
 * Performs a dictionary attack on the given hash
 * @param hash Hash to attack
 * @param hashed_dict Hashed dictionary to use
 * @return Index of the password in the dictionary, -1 if not found
 */
int dictionary_attack(const std::string& hash, const std::vector<std::string>& hashed_dict) {
    for (int i = 0; i < hashed_dict.size(); ++i) {
        if (hashed_dict[i] == hash)
            return i;
    }
    return -1;
}

/**
 * Generates all possible combinations of the given length on the given characters
 * @param length Length of the combinations
 * @param start Starting string
 * @param possible_chars Possible characters to use
 * @return Vector of strings containing all possible combinations
 */
std::vector<std::string> get_all_combinations(int length, const std::string& start = "", std::string_view possible_chars = POSSIBLE_NUMS_CHARS) {
    std::vector<std::string> combinations;
    combinations.emplace_back(start);

    for (int i = 0; i < length; i++) {
        std::vector<std::string> new_combinations;
        for (const auto& combination : combinations) {
            for (const auto& possible_char : possible_chars) {
                new_combinations.emplace_back(combination + possible_char);
            }
        }
        combinations = new_combinations;
    }

    return combinations;
}

/**
 * Performs a brute force attack on the given hash
 * @param hash Hash to attack
 * @param attempts Number of attempts
 * @param possible_chars Possible characters to use
 */
void brute_force_attack(const std::string& hash, long long *attempts, std::string_view possible_chars = POSSIBLE_NUMS_CHARS) {
    std::vector<std::string> combinations;
    for (int i = 1; i < 5; i++) { // Length of 1 to 4
        std::cout << "Trying all combinations - length " << i << "..." << std::endl;
        combinations = get_all_combinations(i, "", possible_chars);
        for (const auto& combination : combinations) {
            mtx.lock();
            if (found) {
                mtx.unlock();
                return;
            }
            (*attempts)++;
            if (sha256(combination + "\n") == hash) {
                found = true;
                bf_found_result = combination;
                mtx.unlock();
                return;
            }
            mtx.unlock();
        }
    }
    for (const auto& possible_char : possible_chars) { // Length of 5
        std::cout << "Trying all combinations - length 5 and starting with " << possible_char << "..." << std::endl;
        combinations = get_all_combinations(4, std::string(1, possible_char), possible_chars);
        for (const auto& combination : combinations) {
            mtx.lock();
            if (found) {
                mtx.unlock();
                return;
            }
            (*attempts)++;
            if (sha256(combination + "\n") == hash) {
                found = true;
                bf_found_result = combination;
                mtx.unlock();
                return;
            }
            mtx.unlock();
        }
    }
    for (const auto& possible_char : possible_chars) { // Length of 6
        for (const auto& possible_char2 : possible_chars) {
            std::cout << "Trying all combinations - length 6 and starting with " << possible_char << possible_char2 << "..." << std::endl;
            combinations = get_all_combinations(4, std::string(1, possible_char) + possible_char2, possible_chars);
            for (const auto& combination : combinations) {
                mtx.lock();
                if (found) {
                    mtx.unlock();
                    return;
                }
                (*attempts)++;
                if (sha256(combination + "\n") == hash) {
                    found = true;
                    bf_found_result = combination;
                    mtx.unlock();
                    return;
                }
                mtx.unlock();
            }
        }
    }
}

/**
 * Main function
 * @return Exit code
 */
int main() {
    std::cout << "Loading password dictionary..." << std::endl;
    auto pass_dict = get_pass_dict();
    std::cout << "Hashing password dictionary..." << std::endl;
    auto pass_dict_hashes = sha256(pass_dict);
    std::cout << "Loading my hashes..." << std::endl;
    auto my_hashes = get_hashes();

    std::vector<std::string> cracked_pass;
    std::vector<long long> attempts_vec;
    std::vector<double> times;

    for (const auto& hash : my_hashes) {
        std::cout << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Attacking hash: " << hash << std::endl;
        std::cout << "Dictionary attack..." << std::endl;
        auto index = dictionary_attack(hash, pass_dict_hashes);
        if (index != -1) {
            auto result = pass_dict[index];
            result.erase(std::remove_if(result.begin(), result.end(),
                                        [](char c){
                                            return std::isspace(static_cast<unsigned char>(c));
                                        }), result.end());
            cracked_pass.push_back(result);
            std::cout << "Password found: " << cracked_pass.back() << std::endl;
            auto end = std::chrono::high_resolution_clock::now();
            times.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0);
            std::cout << "Time taken: " << times.back() << "s" << std::endl;
            attempts_vec.push_back(index + 1);
            std::cout << "Attempts: " << attempts_vec.back() << std::endl;
            continue;
        }
        std::cout << "Brute force attack..." << std::endl;
        long long attempts = 0;
        found = false;
        bf_found_result = "";
        std::thread t1(brute_force_attack, hash, &attempts, POSSIBLE_NUMS);
        std::thread t2(brute_force_attack, hash, &attempts, POSSIBLE_CHARS);
        std::thread t3(brute_force_attack, hash, &attempts, POSSIBLE_NUMS_CHARS);
        std::thread t4(brute_force_attack, hash, &attempts, POSSIBLE_CHARS_NUMS);
        while(!found) {
            auto now = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            if (time > TIME_LIMIT_IN_SECONDS) {
                std::cout << "Time limit exceeded" << std::endl;
                mtx.lock();
                found = true;
                mtx.unlock();
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (t1.joinable()) t1.join();
            if (t2.joinable()) t2.join();
            if (t3.joinable()) t3.join();
            if (t4.joinable()) t4.join();
        }
        if (t1.joinable()) t1.join();
        if (t2.joinable()) t2.join();
        if (t3.joinable()) t3.join();
        if (t4.joinable()) t4.join();
        std::string bf_result = bf_found_result;
        bf_result.erase(std::remove_if(bf_result.begin(), bf_result.end(),
                                       [](char c){
                                           return std::isspace(static_cast<unsigned char>(c));
                                       }), bf_result.end());
        cracked_pass.push_back(bf_result.empty() ? "not_cracked" : bf_result);
        if (cracked_pass.back() != "not_cracked")
            std::cout << "Password found: " << cracked_pass.back() << std::endl;
        else
            std::cout << "Password not cracked" << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        times.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0);
        std::cout << "Time taken: " << times.back() << "s" << std::endl;
        attempts_vec.push_back(attempts);
        std::cout << "Attempts: " << attempts_vec.back() << std::endl;
    }

    std::ofstream output((std::string("cracked_results_" + std::string(MY_USERNAME) + ".csv")));
    for (int i = 0; i < cracked_pass.size(); ++i)
        output << my_hashes[i] << ";" << cracked_pass[i] << ";" << attempts_vec[i] << ";" << times[i] << std::endl;

    return EXIT_SUCCESS;
}
