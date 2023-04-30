#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <regex>

/** Image to use for steganography */
constexpr std::string_view STEGANOGRAPHY_IMG = "weber.bmp";
/** Directory with files to hide */
constexpr std::string_view DATA_SOURCE = "validation/";
/** Directory with encoded files */
constexpr std::string_view OUTPUT_DIR = "out/";
/** Directory with decoded files */
constexpr std::string_view DECODED_DIR = "decoded/";
/** Delimeter between filename, file extension and file size */
constexpr std::string_view DELIMETER = "___";
/** BMP format header size */
constexpr int BMP_HEADER_SIZE = 54;
/** Number of bits in a byte */
constexpr int BITS_IN_BYTE = 8;
/** Size of metadata in bytes (extension, size) */
constexpr int METADATA_SIZE = 8 + 8;

/**
 * Function determines if the input file is about to fit into STEGANOGRAPHY_IMG
 * @param steganography_img_size Number of bytes of STEGANOGRAPHY_IMG
 * @param input_file_size Number of bytes of an input file
 * @return True if input file is too big and cannot fit, False otherwise
 */
bool is_file_too_big(size_t steganography_img_size, size_t input_file_size) {
    return steganography_img_size - BMP_HEADER_SIZE < BITS_IN_BYTE * (input_file_size + METADATA_SIZE);
}

/**
 * This procedure performs steganography with the LSB method
 * The part of a filepath is a filename and also a file extension
 * @param input_file The file which is about to hide into STEGANOGRAPHY IMG
 * @param output_file Path to the encoded img with a hidden file
 * @param extension File extension
 * @param size File size
 */
void hide(const std::string &input_file, const std::string &output_file, std::string extension, int size) {
    std::ifstream image(std::string(STEGANOGRAPHY_IMG), std::ios::binary);
    std::ifstream input(input_file, std::ios::binary);

    std::vector<unsigned char> image_data((std::istreambuf_iterator<char>(image)), std::istreambuf_iterator<char>());
    std::vector<unsigned char> input_data((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

    if (is_file_too_big(image_data.size(), input_data.size())) {
        std::cout << "File " << input_file << " is too big for steganography!" << std::endl;
        return;
    }

    /* Copy header */
    std::vector<unsigned char> result_bytes;
    for (int i = 0; i < BMP_HEADER_SIZE; i++)
        result_bytes.push_back(image_data[i]);

    /* Encode extension */
    for (int i = 0; i < METADATA_SIZE / 2; i++) {
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            if (i < extension.size())
                result_bytes.push_back(
                    (image_data[i * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0xFE) | ((extension[i] >> j) & 0x01));
            else
                result_bytes.push_back((image_data[i * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0xFE));
        }
    }

    /* Encode size */
    char * size_bytes = (char *) &size;
    for (int i = METADATA_SIZE / 2; i < METADATA_SIZE; i++) {
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            result_bytes.push_back(
                    (image_data[i * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0xFE) | ((size_bytes[i - METADATA_SIZE / 2] >> j) & 0x01));
        }
    }

    /* Encode input file */
    for (int i = 0; i < input_data.size(); i++) {
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            result_bytes.push_back(
                    (image_data[(i + METADATA_SIZE) * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0xFE) | ((input_data[i] >> j) & 0x01));
        }
    }

    /* Copy the rest of the image */
    for (int i = (static_cast<int>(input_data.size()) + METADATA_SIZE) * BITS_IN_BYTE + BMP_HEADER_SIZE; i < image_data.size(); i++)
        result_bytes.push_back(image_data[i]);

    std::ofstream output(output_file, std::ios::binary);
    output.write((char *) result_bytes.data(), static_cast<int>(result_bytes.size()));
}

/**
 * This method decodes a hidden file from a filepath given as parameter and stores it into decoded folder.
 * The part of a filepath is a filename and also a file extension
 * @param file The file which is about to be decoded and stored into decoded folder
 */
void decode(const std::string &file) {
    std::ifstream image(file, std::ios::binary);

    std::vector<unsigned char> image_data((std::istreambuf_iterator<char>(image)), std::istreambuf_iterator<char>());

    auto basename_filepath = file.substr(file.find_last_of("/\\") + 1);
    std::regex regex("(.*)" + std::string(DELIMETER) + "(.*)");
    std::smatch match;
    std::regex_match(basename_filepath, match, regex);
    std::string filename = match[1];

    std::string extension;
    int size = 0;

    /* Decode extension */
    for (int i = 0; i < METADATA_SIZE / 2; i++) {
        char byte = 0;
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            byte |= ((image_data[i * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0x01) << j);
        }
        extension += byte;
    }

    /* Decode size */
    char * size_bytes = (char *) &size;
    for (int i = METADATA_SIZE / 2; i < METADATA_SIZE; i++) {
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            size_bytes[i - METADATA_SIZE / 2] |= ((image_data[i * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0x01) << j);
        }
    }

    auto output_file = std::string(DECODED_DIR) + filename + "." + extension;

    /* Decode input file */
    std::vector<unsigned char> result_bytes;
    for (int i = 0; i < size; i++) {
        unsigned char byte = 0;
        for (int j = 0; j < BITS_IN_BYTE; j++) {
            byte |= ((image_data[(i + METADATA_SIZE) * BITS_IN_BYTE + j + BMP_HEADER_SIZE] & 0x01) << j);
        }
        result_bytes.push_back(byte);
    }

    std::ofstream output(output_file, std::ios::binary);
    output.write((char *) result_bytes.data(), static_cast<int>(result_bytes.size()));
}

/**
 * Main function
 */
int main() {
    if (!std::filesystem::exists(DECODED_DIR))
        std::filesystem::create_directory(DECODED_DIR);

    /* 1. Phase Hiding (Encoding) -- Steganography */
    if (std::filesystem::exists(DATA_SOURCE)) {
        std::vector<std::string> files;
        for (const auto &entry: std::filesystem::directory_iterator(DATA_SOURCE))
            files.push_back(entry.path().string());

        for (const auto &file: files) {
            std::cout << "Hiding file " << file << " into " << STEGANOGRAPHY_IMG << std::endl;
            auto filename = file.substr(file.find_last_of("/\\") + 1);
            auto filename_without_extension = filename.substr(0, filename.find_last_of('.'));
            auto extension = filename.substr(filename.find_last_of('.') + 1);
            int size = static_cast<int>(std::filesystem::file_size(file));
            auto output_file = std::string(OUTPUT_DIR)
                    .append(filename_without_extension)
                    .append(DELIMETER)
                    .append(STEGANOGRAPHY_IMG);
            std::cout << "Output file: " << output_file << std::endl;
            hide(file, output_file, extension, size);
        }
    }

    /* 2. Phase Decoding */
    if (std::filesystem::exists(OUTPUT_DIR)) {
        std::vector<std::string> files;
        for (const auto &entry: std::filesystem::directory_iterator(OUTPUT_DIR))
            files.push_back(entry.path().string());

        for (const auto &file: files) {
            std::cout << "Decoding a hidden file from " << file << std::endl;
            decode(file);
        }
    }

    return EXIT_SUCCESS;
}
