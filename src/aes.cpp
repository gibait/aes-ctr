#include "cipher_operations.h"
#include "aes.h"
#include "utils.h"
#include "core.cpp"

#include <array>
#include <thread>
#include <vector>
#include <cmath>

/*
 * @brief: Split a string into vector of strings (chunks)
 * @param plaintext: plaintext string
 * @param chunks: vector of strings
 * @param num_threads: number of threads
 * @param chunks_for_every_thread: number of chunks for every thread
 */
void split_plaintext_into_chunks(std::string& plaintext,
                                 std::vector<std::string>& chunks,
                                 size_t num_threads,
                                 size_t chunks_for_every_thread)
{
    chunks.clear();

    for (int i = 0; i < num_threads; ++i) {
        if (i == num_threads - 1)
            chunks.emplace_back(plaintext.substr(i * chunks_for_every_thread * BLOCK_SIZE, plaintext.size()));
        else
            chunks.emplace_back(plaintext.substr(i * chunks_for_every_thread * BLOCK_SIZE, chunks_for_every_thread * BLOCK_SIZE));
    }
}

/*
 * @brief: Encrypts or decrypts the given string based on the mode
 * @param plaintext: plaintext string
 * @param key: key
 * @param m: mode
 * @param output: output string
 * @return: 0 on success, 1 on failure
 */
int cipher::AES::encrypt(std::string &plaintext,
                         k& key,
                         mode m,
                         std::string &output)
{
    D(std::cerr << "------------------------------------------" << std::endl);

    // Clear the output string
    output.clear();
    // Reset the counter
    counter[1] = 1;

    exp_k expanded_key;
    cipher::_key_expansion(key, expanded_key);

    // Get plaintext size and chunks sizes
    size_t size = plaintext.size();
    D(std::cerr << "plaintext size: " << size << std::endl);
    // Total number of (16 byte) chunks
    size_t num_of_chunks = std::ceil(double(size) / double(BLOCK_SIZE));
    D(std::cerr << "num_of_chunks: " << num_of_chunks << std::endl);

    // Where to store the encrypted chunks of data
    // Initialized to the number of chunks
    std::vector<std::string> ciphertext(num_of_chunks);

    if (m == SINGLE) {

        cipher_fn(plaintext, counter, expanded_key, ciphertext);
        D(std::cerr << "ciphertext (single thread): " << output << std::endl);

    } else {

        // Decrement if the size is too small
        while (size < BLOCK_SIZE * num_threads) {
            num_threads -= 1;
        }
        num_threads = num_threads == 0 ? 1 : num_threads;
        D(std::cerr << "num_threads: " << num_threads << std::endl);

        // Number of chunks for every thread
        size_t chunks_for_every_thread = num_of_chunks / num_threads;
        D(std::cerr << "chunks_for_every_thread: " << chunks_for_every_thread << std::endl);

        // Split the input into chunks
        std::vector<std::string> chunks;
        split_plaintext_into_chunks(plaintext, chunks, num_threads, chunks_for_every_thread);

        std::vector<std::thread> threads;

        // Create the threads
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(&AES::cipher_fn,
                                 this,
                                 std::ref(chunks[i]),
                                 counter,
                                 std::ref(expanded_key),
                                 std::ref(ciphertext));
            counter[1] += chunks_for_every_thread;
        }

        // Join the threads
        for (auto &thread: threads) {
            thread.join();
        }

        D(std::cerr << "ciphertext (multi thread): " << output << std::endl);
    }

    // Convert ciphertext to string
    for (auto &c: ciphertext) {
        output.append(c);
    }

    return 0;
}