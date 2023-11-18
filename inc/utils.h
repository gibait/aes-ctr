#ifndef AES_CTR_128_THREADED_UTILS_H
#define AES_CTR_128_THREADED_UTILS_H

#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

/*
 * @brief: Generates a random 64-bit nonce using std::random_device
 * @return: 64-bit nonce
 */
inline uint64_t get_nonce()
{

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    return dist(rng);
}

/*
 * @brief: Loads the contents of a file into a string
 * @param file: file stream
 */
inline std::string load_file_into_string(std::ifstream& file)
{
    return {(std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()};
}

/*
 * @brief: Writes a string to a file
 * @param file: file stream
 * @param str: string to write
 */
inline void write_string_to_file(std::ofstream& file, std::string& str)
{
    file.write(str.c_str(), str.size());
}

#endif //AES_CTR_128_THREADED_UTILS_H
