#include "aes.h"
#include "cipher_operations.h"
#include "utils.h"

#include <thread>
#include <vector>
#include <mutex>
#include <sstream>

/*
 * @brief: Encrypts a block of 128 bits
 * @param counter: counter
 * @param round_key: round key
 * @param state: state
 */
void cipher::AES::encrypt_block(const ctr& counter,
                                const exp_k& expanded_key,
                                blk& state)
{
    std::copy(begin(counter), end(counter), begin(state[0]));

    std::lock_guard<std::mutex> lock(expanded_key_mtx);
    cipher::_add_round_key(state, expanded_key[0]);
    for (int i = 0; i < 9; ++i) {
        cipher::_sub_bytes(state);
        cipher::_shift_rows(state);
        cipher::_mix_columns(state);
        cipher::_add_round_key(state, expanded_key[i - 1]);
    }
    cipher::_sub_bytes(state);
    cipher::_shift_rows(state);
    cipher::_add_round_key(state, expanded_key[10]);
}

/*
 * @brief: Encrypts a string using AES-128 in CTR mode
 * @param plaintext: plaintext string
 * @param counter: block cipher input
 * @param exp_k: expanded keys
 * @param ciphertext: vector of ciphertext strings
 *
 * @return: 0 on success, 1 on failure
 */
int cipher::AES::cipher_fn(std::string& plaintext,
                           ctr counter,
                           const exp_k& expanded_key,
                           std::vector<std::string>& ciphertext)
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    D(std::cerr << "pthread_id: " << oss.str() << std::endl);
    D(std::cerr << "\tplaintext: " << plaintext << std::endl);
    D(std::cerr << "\tnonce: " << counter[0] << std::endl);
    D(std::cerr << "\tcounter: " << counter[1] << std::endl);

    size_t remaining = plaintext.size();
    auto it = plaintext.begin();

    while (remaining) {

        // Determine how many bytes to write
        size_t to_write = remaining > BLOCK_SIZE ? BLOCK_SIZE : remaining;

        // Read from plaintext 128 bits or fewer
        blk in{};
        std::copy(it, it + to_write, in[0].begin());
        it += to_write;

        // Encrypt the block
        blk state{};
        cipher::AES::encrypt_block(counter, expanded_key, state);

        // XOR only the plaintext bits size of state with
        uint8_t *in_ptr = &in[0][0];
        uint8_t *state_ptr = &state[0][0];
        for (int i = 0; i < to_write; ++i) {
            state_ptr[i] ^= in_ptr[i];
        }

        // Write the ciphered string to the ciphertext vector
        std::lock_guard<std::mutex> lock(ciphertext_mtx);
        ciphertext[counter[1] - 1].append(reinterpret_cast<char *>(state_ptr), to_write);

        // Increment the counter
        counter[1] += 1;

        remaining -= to_write;
    }

    return 0;
}