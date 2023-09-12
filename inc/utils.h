#ifndef AES_CTR_128_THREADED_UTILS_H
#define AES_CTR_128_THREADED_UTILS_H

#include <cstdint>
#include <fstream>
#include <iostream>

#ifdef DEBUG
#define D(x) x
#else
#define D(x)
#endif

/*
 * @brief: Generates a random 64-bit nonce reading from /dev/urandom
 * @return: 64-bit nonce
 */
inline uint64_t get_nonce()
{

    uint64_t nonce;
    std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
    if (!urandom) {
        std::cerr << "Error: could not open /dev/urandom" << std::endl;
        exit(1);
    }

    urandom.read(reinterpret_cast<char *>(&nonce), sizeof(nonce));
    if (nonce == 0) {
        std::cerr << "Error: nonce cannot be 0" << std::endl;
        exit(1);
    }
    return nonce;
}

/*
 * @brief: Loads the contents of a file into a string
 * @param file: file stream
 */
inline std::string load_file_into_string(std::ifstream& file)
{
    std::string str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    return str;
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
