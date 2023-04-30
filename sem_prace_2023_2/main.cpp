#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

/** Directory with files to encode */
constexpr std::string_view DATA_SOURCE = "validation/";
/** Directory with encoded files */
constexpr std::string_view OUTPUT_DIR = "out/";
/** Directory with decoded files */
constexpr std::string_view DECODED_DIR = "decoded/";

/**
 * Loads Feistel key from file
 * @param key_filepath Filepath to key file
 * @param block_size Block size in bits (output)
 * @return Key as vector of unsigned ints
 */
std::vector<unsigned int> load_feistel_key(const std::string &key_filepath, int *block_size) {
    std::ifstream key_file(key_filepath);
    if (!key_file.is_open()) {
        std::cout << "Unable to open key file " << key_filepath << std::endl;
        return {};
    }

    // Check the format
    std::string line;
    std::getline(key_file, line);
    auto key_size = 0;
    for ([[maybe_unused]] auto &c: line) key_size++;
    if (key_size < 0 || key_size % 4 != 0 || key_size > 32) {
        std::cout << "Invalid key size " << key_size << std::endl;
        return {};
    }

    // Check the format of all lines
    while (std::getline(key_file, line)) {
        auto key_size_tmp = 0;
        std::vector<int> key;
        for ([[maybe_unused]] auto &c: line) key_size_tmp++;

        if (key_size_tmp < 0 || key_size_tmp % 4 != 0 || key_size_tmp > 32 || key_size_tmp != key_size) {
            std::cout << "Invalid key size " << key_size_tmp << std::endl;
            return {};
        }
    }

    // "Return" block size as key size * 2 (left and right parts of block) (in bits)
    *block_size = key_size * 2;

    // Return to the beginning of the file
    key_file.clear();
    key_file.seekg(0);

    std::vector<unsigned int> key;
    while (std::getline(key_file, line)) {
        unsigned int tmp = 0;
        for (auto &c: line) {
            if (c == '0')
                tmp = tmp << 1;
            else if (c == '1')
                tmp = (tmp << 1) | 1;
            else {
                std::cout << "Invalid key character " << c << std::endl;
                return {};
            }
        }
        key.push_back(tmp);
    }

    return key;
}

/**
 * Feistel encoding/decoding
 * @param input_bytes Input bytes
 * @param encode Flag indicating whether to encode or decode
 * @return Output bytes
 */
std::vector<unsigned char> perform_feistel(const std::vector<unsigned char> &input_bytes, bool encode) {
    int block_size_bits = 0;
    std::vector<unsigned int> key = load_feistel_key("keys.txt", &block_size_bits);
    if (key.empty()) // If key format is invalid, return empty vector
        return {};

    // Pad input bytes with zeros to make it a multiple of block size (in bytes)
    std::vector<unsigned char> input_bytes_padded;
    input_bytes_padded.insert(input_bytes_padded.end(), input_bytes.begin(), input_bytes.end());
    while (input_bytes_padded.size() % (block_size_bits / 8)) // block_size_bits / 8 = block size in bytes
        input_bytes_padded.push_back(0x00);

    std::vector<unsigned char> output_bytes;
    if (encode) {
        std::cout << "Key has been loaded. Performing encoding... Block size = " << block_size_bits << " bits"
                  << std::endl;
    } else {
        std::cout << "Key has been loaded. Performing decoding... Block size = " << block_size_bits << " bits"
                  << std::endl;
        std::reverse(key.begin(), key.end()); // Reverse key for decoding
    }

    // Iterate over all blocks of input bytes
    for (int i = 0; i < input_bytes_padded.size() * 8; i += block_size_bits) {
        unsigned int left = 0;
        unsigned int right = 0;
        for (int j = 0; j < block_size_bits / 2; j++) { // Accumulate left and right parts of block
            left = (left << 1) | (input_bytes_padded[i / 8 + j / 8] & (1 << (7 - j % 8)) ? 1 : 0);
            right = right << 1 | (input_bytes_padded[i / 8 + (j + block_size_bits / 2) / 8] &
                                  (1 << (7 - (j + block_size_bits / 2) % 8)) ? 1 : 0);
        }

        // Perform Feistel rounds
        for (unsigned int subkey: key) {
            unsigned int tmp = left;
            left = right;
            right = tmp ^ ((right ^ subkey) & subkey);
        }

        // Write output bits
        std::vector<unsigned char> output_bits;
        for (int j = block_size_bits / 2 - 1; j >= 0; j--)
            output_bits.push_back(right & (1 << j) ? 0x01 : 0x00); // Right first for the final swap
        for (int j = block_size_bits / 2 - 1; j >= 0; j--)
            output_bits.push_back(left & (1 << j) ? 0x01 : 0x00); // Left second because of the final swap

        // Convert output bits to bytes
        for (int j = 0; j < output_bits.size(); j += 8) {
            unsigned char byte = 0;
            for (int k = 0; k < 8; k++)
                byte = (byte << 1) | output_bits[j + k];
            output_bytes.push_back(byte);
        }
    }

    // Remove trailing zeros if decoding (they were added for padding)
    if (!encode) {
        while (!output_bytes.back())
            output_bytes.pop_back();
    }

    return output_bytes;
}

/**
 * Main function
 * @return 0 if successful
 */
int main() {
    // If directory with data exists, process all files in it
    if (std::filesystem::exists(DATA_SOURCE)) {
        std::vector<std::string> files;
        for (const auto &entry: std::filesystem::directory_iterator(DATA_SOURCE))
            files.push_back(entry.path().string());
        std::sort(files.begin(), files.end()); // Python example had them sorted, I don't know why this is needed?

        for (const auto &filepath: files) {
            auto basename_filepath = filepath.substr(filepath.find_last_of("/\\") + 1); // filename.extension
            auto filename = basename_filepath.substr(0, basename_filepath.find_last_of('.')); // filename
            auto extension = basename_filepath.substr(basename_filepath.find_last_of('.') + 1); // extension

            auto input_bytes = std::vector<unsigned char>();
            std::ifstream input(filepath, std::ios::binary);
            std::copy(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(),
                      std::back_inserter(input_bytes));

            if (input_bytes.empty()) {
                std::cout << "File " << filepath << " is empty" << std::endl;
                continue;
            }

            std::cout << "Encrypting file " << filepath << " bytes..." << std::endl;
            auto hex_out_filename = std::string(OUTPUT_DIR).append(filename).append("_hexoutput.txt");
            auto encoded_filepath = std::string(OUTPUT_DIR).append(filename).append(".bin");
            auto decoded_filepath = std::string(DECODED_DIR).append(filename).append(".").append(extension);

            // Encoding
            auto encoded_bytes = perform_feistel(input_bytes, true);
            if (!encoded_bytes.empty()) {
                std::ofstream output(encoded_filepath, std::ios::binary);
                output.write((char *) encoded_bytes.data(), static_cast<int>(encoded_bytes.size()));
            }

            // Decoding
            auto decoded_bytes = perform_feistel(encoded_bytes, false);
            if (!decoded_bytes.empty()) {
                std::ofstream output_hex(hex_out_filename, std::ios::binary);
                for (int i = 0; i < 100 && i < input_bytes.size(); i++)
                    output_hex << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(input_bytes.at(i));
                output_hex << std::endl;
                for (int i = 0; i < 100 && i < encoded_bytes.size(); i++)
                    output_hex << std::hex << std::setfill('0') << std::setw(2)
                               << static_cast<int>(encoded_bytes.at(i));
                output_hex << std::endl;
                for (int i = 0; i < 100 && i < decoded_bytes.size(); i++)
                    output_hex << std::hex << std::setfill('0') << std::setw(2)
                               << static_cast<int>(decoded_bytes.at(i));

                std::ofstream decoded(decoded_filepath, std::ios::binary);
                decoded.write((char *) decoded_bytes.data(), static_cast<int>(decoded_bytes.size()));
            }
        }
    }

    return EXIT_SUCCESS;
}
