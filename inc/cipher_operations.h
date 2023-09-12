#ifndef CIPHER_OPERATIONS_H
#define CIPHER_OPERATIONS_H

#include <cstdint>
#include <bitset>

#define WORD_SIZE 4
#define NUM_ROUNDS 10

using wd = std::array<uint8_t, WORD_SIZE>;
using blk = std::array<wd, WORD_SIZE>;
using k = std::array<wd, WORD_SIZE>;
using exp_k = std::array<k, NUM_ROUNDS + 1>;

namespace cipher
{

    /*
     * SUBBYTES () is an invertible, non-linear transformation of the state in which a substitution table,
     * called an S-box, is applied independently to each byte in the state. The AES S-box is denoted by
     * SBOX.
     */
    void _sub_bytes(
            blk& state
    );

    /*
     * SHIFTROWS () is a transformation of the state in which the bytes in the last three rows of the state
     * are cyclically shifted. The number of positions by which the bytes are shifted depends on the row
     * index r, with 0 ≤ r ≤ 3. The first row, r = 0, is not shifted. The second row, r = 1, is shifted
     */
    void _shift_rows(
            blk &state
    );

    /*
     * MIXCOLUMNS () is a transformation of the state that multiplies each of the four columns of the
     * state by a single fixed matrix
     */
    void _mix_columns(
            blk &state
    );

    /*
     * ADDROUNDKEY () is a transformation of the state in which a round k is combined with the
     * state by applying the bitwise XOR operation. In particular, each round k consists of four words
     * from the k schedule, each of which is combined with a column of the state
     */
    void _add_round_key(
            blk &state,
            const k &round_key
    );

    /*
     * KEYEXPANSION () is a routine that is applied to the k to generate 4 ∗ (Nr + 1) words. Thus,
     * four words are generated for each of the Nr + 1 applications of ADDROUNDKEY () within the
     * specification of CIPHER
     */
    void _key_expansion(
            const k &key,
            exp_k &expanded_key
    );

    /*
     * ROTWORD([a0, a1, a2, a3]) = [a1, a2, a3, a0]
     */
    void _rot_word(
            wd &word
    );

    /*
     * SUBWORD([a0, . . . , a3]) = [SBOX(a0), SBOX(a1), SBOX(a2), SBOX(a3)]
     */
    void _sub_word(
            wd &word
    );

    void _xor_word(
            wd &word,
            uint8_t round_constant
    );

} // namespace cipher

#endif //CIPHER_OPERATIONS_H