#include <algorithm>
#include <array>
#include "../inc/cipher_operations.h"
#include "../inc/lookup_tables.h"

void cipher::_sub_bytes(blk &state)
{
    for (int r = 0; r < WORD_SIZE; ++r) {
        for (int c = 0; c < WORD_SIZE; ++c) {
            state[r][c] = S_BOX[state[r][c]];
        }
    }
}

void cipher::_shift_rows(blk &state)
{
    wd cur_row;

    for (int r = 1; r < WORD_SIZE; ++r) {
        std::copy(state[r].begin(), state[r].end(), cur_row.begin());

        for (int c = 0; c < WORD_SIZE; ++c) {
            // Shift the row by r positions
            state[r][c] = cur_row[(c + r) % WORD_SIZE];
        }
    }
}

void cipher::_rot_word(wd &word)
{
    uint8_t temp = word[0];

    for (int i = 0; i < WORD_SIZE - 1; i++) {
        word[i] = word[i + 1];
    }

    word[WORD_SIZE - 1] = temp;
}

void cipher::_sub_word(wd &word)
{
    for (int r = 0; r < WORD_SIZE; ++r) {
        word[r] = S_BOX[word[r]];
    }
}

void cipher::_xor_word(wd &word,
                       const uint8_t constant)
{
    for (int r = 0; r < WORD_SIZE; ++r) {
        word[r] ^= constant;
    }
}

void cipher::_mix_columns(blk &state)
{
    wd cur_col;

    for (int c = 0; c < WORD_SIZE; ++c) {
        std::copy(state[c].begin(), state[c].end(), cur_col.begin());

        // Using hardcoded GF(2) multiplication tables
        state[0][c] = (uint8_t) (MUL_2[cur_col[0]] ^ MUL_3[cur_col[1]] ^ cur_col[2] ^ cur_col[3]);
        state[1][c] = (uint8_t) (cur_col[0] ^ MUL_2[cur_col[1]] ^ MUL_3[cur_col[2]] ^ cur_col[3]);
        state[2][c] = (uint8_t) (cur_col[0] ^ cur_col[1] ^ MUL_2[cur_col[2]] ^ MUL_3[cur_col[3]]);
        state[3][c] = (uint8_t) (MUL_3[cur_col[0]] ^ cur_col[1] ^ cur_col[2] ^ MUL_2[cur_col[3]]);
    }
}

void cipher::_add_round_key(blk &state,
                            const k &round_key)
{
    for (int r = 0; r < WORD_SIZE; ++r) {
        for (int c = 0; c < WORD_SIZE; ++c) {
            state[r][c] ^= round_key[r][c];
        }
    }
}

void cipher::_key_expansion(const k &key,
                            exp_k &expanded_key)
{
    // Array of words, each word is a 4-byte array
    using expanded = std::array<wd, WORD_SIZE * (NUM_ROUNDS + 1)>;
    expanded tmp_exp {};

    // Copy the k to the first 4 words of the expanded k
    for (int i = 0; i < WORD_SIZE; ++i) {
        std::copy(key.begin(), key.end(), tmp_exp.begin());
    }

    // Generate the rest of the expanded k
    wd temp{};
    for (int i = WORD_SIZE; i < (NUM_ROUNDS+1) * WORD_SIZE; ++i) {

        // Copy previous word into temp
        std::copy(tmp_exp[i - 1].begin(),
                  tmp_exp[i - 1].end(),
                  temp.begin());

        if (i % WORD_SIZE == 0) {
            _rot_word(temp);
            _sub_word(temp);
            _xor_word(temp, RCON[i / WORD_SIZE]);
        } else {
            _sub_word(temp);
        }

        for (int r = 0; r < WORD_SIZE; ++r) {
            tmp_exp[i][r] = tmp_exp[i - WORD_SIZE][r] ^ temp[r];
        }
    }

    // Convert the 44 words into 11 round keys
    for (int i = 0; i < NUM_ROUNDS + 1; ++i) {
        for (int r = 0; r < WORD_SIZE; ++r) {
            for (int c = 0; c < WORD_SIZE; ++c) {
                expanded_key[i][r][c] = tmp_exp[i * WORD_SIZE + r][c];
            }
        }
    }
}