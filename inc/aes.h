#ifndef AES_CTR_128_THREADED_AES_H
#define AES_CTR_128_THREADED_AES_H

#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include "cipher_operations.h"
#include "utils.h"

#define BLOCK_SIZE 16

typedef enum {
    SINGLE,
    MULTI
} mode;

using ctr = std::array<uint64_t, 2>;

namespace cipher {

    class AES {
    public:

        AES() {
            counter[0] = get_nonce();
            counter[1] = 1;
        }

        /*
         * @brief: Encrypts the given string using given key in CTR mode
         * @param plaintext: plaintext string
         * @param key: key
         * @param ciphertext: ciphertext string
         */
        int encrypt(
                std::string &plaintext,
                k& key,
                std::string &ciphertext) {
            return encrypt(plaintext, key, SINGLE, ciphertext);
        }

        /*
         * @brief: Decrypts the given string using given key in CTR mode
         * @param ciphertext: ciphertext string
         * @param key: key
         * @param plaintext: plaintext string
         */
        int decrypt(
                std::string &ciphertext,
                k& key,
                std::string &plaintext) {
            return encrypt(ciphertext, key, SINGLE, plaintext);
        }

        /*
         * @brief: Encrypts the given string using given key in CTR mode
         *        using multiple threads
         * @param plaintext: plaintext string
         * @param key: key
         * @param ciphertext: ciphertext string
         */
        int encrypt_multi_thread(
                std::string &plaintext,
                k& key,
                std::string &ciphertext) {
            return encrypt(plaintext, key, MULTI, ciphertext);
        }

        /*
         * @brief: Decrypts the given string using given key in CTR mode
         *       using multiple threads
         * @param ciphertext: ciphertext string
         * @param key: key
         * @param plaintext: plaintext string
         */
        int decrypt_multi_thread(
                std::string &ciphertext,
                k& key,
                std::string &plaintext) {
            return encrypt(ciphertext, key, MULTI, plaintext);
        }

    private:

        std::mutex expanded_key_mtx, ciphertext_mtx;
        ctr counter{};

        // Encrypt 16 bytes block
        void encrypt_block(
                const ctr& counter,
                const exp_k& expanded_key,
                blk& state);

        // Cipher function for single thread
        int cipher_fn(
                std::string& plaintext,
                ctr counter,
                const exp_k& expanded_key,
                std::vector<std::string>& ciphertext);

        // Encrypts or decrypts the given string in single or multi-thread mode
        int encrypt(
                std::string &input,
                k& key,
                mode m,
                std::string &output);
    };
}

#endif //AES_CTR_128_THREADED_AES_H
