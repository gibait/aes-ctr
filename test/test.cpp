#include "../inc/aes.h"
#include <cassert>

std::ofstream os("single-multi-comparison.txt", std::ios::out);
std::ofstream os2("threads-comparison.txt", std::ios::out);

k key = {0x23, 0x45, 0x67, 0x89,
         0xab, 0xcd, 0xef, 0x01,
         0x23, 0x45, 0x67, 0x89,
         0xab, 0xcd, 0xef, 0x01};

void test_st_vs_mt(std::string& file)
{

    cipher::AES aes(4);
    std::string ciphertext;
    std::string deciphered{};
    std::ifstream plaintext_file_stream(file, std::ios::in | std::ios::binary);
    std::string plaintext = load_file_into_string(plaintext_file_stream);

    /*
     * -----------------------------------------
     *             SINGLE THREAD
     * -----------------------------------------
     */

    auto start = std::chrono::high_resolution_clock::now();
    aes.encrypt(plaintext, key, ciphertext);
    aes.decrypt(ciphertext, key, deciphered);
    auto end = std::chrono::high_resolution_clock::now();
    assert(plaintext == deciphered);


    /*
     * -----------------------------------------
     *             MULTI THREAD
     * -----------------------------------------
     */

    ciphertext.clear();
    deciphered.clear();

    auto start_mt = std::chrono::high_resolution_clock::now();
    aes.encrypt_multi_thread(plaintext, key, ciphertext);
    aes.decrypt_multi_thread(ciphertext, key, deciphered);
    auto end_mt = std::chrono::high_resolution_clock::now();
    assert(plaintext == deciphered);


    auto duration_st = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    auto duration_mt = std::chrono::duration_cast<std::chrono::milliseconds>(end_mt - start_mt).count();
    auto speed_up = 100 - ((float(duration_mt) / float(duration_st)) * 100);

    std::cout << "--------------------------------------------\n";
    std::cout << "SINGLE THREAD vs MULTI THREAD ENCRYPTION\n";
    std::cout << "File: " << file << " --> size " << plaintext.size() << " bytes\n";
    std::cout << "Single thread (file) encryption time: " << duration_st << "ms\n";
    std::cout << "Multi thread (file) encryption time: " << duration_mt << "ms\n\n";
    std::cout << "Speed up is " << speed_up << "%\n\n";


    os << plaintext.size() << " " << duration_st << " " << duration_mt << "\n";

}

void test_num_of_threads(std::string& file)
{
    size_t num_threads = 1;
    std::string ciphertext;
    std::string deciphered{};
    std::ifstream plaintext_file_stream(file, std::ios::in | std::ios::binary);
    std::string plaintext = load_file_into_string(plaintext_file_stream);


    do {
        cipher::AES aes(num_threads);
        auto start = std::chrono::high_resolution_clock::now();
        aes.encrypt_multi_thread(plaintext, key, ciphertext);
        aes.decrypt_multi_thread(ciphertext, key, deciphered);
        auto end = std::chrono::high_resolution_clock::now();
        assert(plaintext == deciphered);

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        std::cout << "--------------------------------------------\n";
        std::cout << "THREAD's SPEED COMPARISON ENCRYPTION\n";
        std::cout << "File: " << file << " --> size " << plaintext.size() << " bytes\n";
        std::cout << "Number of threads: " << num_threads << "\n";
        std::cout << "Time: " << duration << " milliseconds \n\n";

        os2 << num_threads << " " << duration << "\n";

        ++num_threads;

    } while (num_threads < std::thread::hardware_concurrency());

}

int main()
{

    std::string file1 = "input/1mb.txt";
    std::string file2 = "input/5mb.txt";
    std::string file3 = "input/10mb.txt";
    std::string file4 = "input/bibbia.txt";


    test_st_vs_mt(file1);
    test_st_vs_mt(file2);
    test_st_vs_mt(file3);
    test_st_vs_mt(file4);

    test_num_of_threads(file3);

    return 0;
}
