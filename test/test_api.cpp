#include "aes.h"

int main()
{
    cipher::AES aes;
    std::string plaintext = "abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {} abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI JKL MNO PQRS TUV WXYZ !\"§ $%& /() =?* '<> #|; ²³~ @`´ ©«» ¤¼× {}abc def ghi jkl mno pqrs tuv wxyz ABC DEF GHI";
    std::string ciphertext;

    k key = {0x23, 0x45, 0x67, 0x89,
             0xab, 0xcd, 0xef, 0x01,
             0x23, 0x45, 0x67, 0x89,
             0xab, 0xcd, 0xef, 0x01};

    // Measure single_thread encryption time
    auto start = std::chrono::high_resolution_clock::now();
    aes.encrypt(plaintext, key, ciphertext);
    aes.decrypt(ciphertext, key, plaintext);
    auto end = std::chrono::high_resolution_clock::now();

    // Measure multi_thread encryption time
    auto start_mt = std::chrono::high_resolution_clock::now();
    aes.encrypt_multi_thread(plaintext, key, ciphertext);
    aes.decrypt_multi_thread(ciphertext, key, plaintext);
    auto end_mt = std::chrono::high_resolution_clock::now();

    std::cout << "Single thread (string) encryption time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << std::endl;
    std::cout << "Multi thread (string) encryption time: " << std::chrono::duration_cast<std::chrono::microseconds>(end_mt - start_mt).count() << "ms" << std::endl;

    std::string file = "plaintext.txt";
    std::string ciphertext2{};
    std::ifstream plaintext_file_stream(file, std::ios::in | std::ios::binary);
    std::string plaintext2 = load_file_into_string(plaintext_file_stream);

    // Measure single_thread file encryption time
    start = std::chrono::high_resolution_clock::now();
    aes.encrypt(plaintext2, key, ciphertext2);
    aes.decrypt(ciphertext2, key, plaintext2);
    end = std::chrono::high_resolution_clock::now();

    std::ofstream ciphertext_file_stream("decrypted.txt", std::ios::out | std::ios::binary);
    write_string_to_file(ciphertext_file_stream, plaintext2);

    // Measure multi_thread file encryption time
    ciphertext2.clear();
    start_mt = std::chrono::high_resolution_clock::now();
    aes.encrypt_multi_thread(plaintext2, key, ciphertext2);
    aes.decrypt_multi_thread(ciphertext2, key, plaintext2);
    end_mt = std::chrono::high_resolution_clock::now();

    std::ofstream ciphertext_file_stream2("decrypted_multi.txt", std::ios::out | std::ios::binary);
    write_string_to_file(ciphertext_file_stream2, plaintext2);

    std::cout << "Single thread (file) encryption time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << std::endl;
    std::cout << "Multi thread (file) encryption time: " << std::chrono::duration_cast<std::chrono::microseconds>(end_mt - start_mt).count() << "ms" << std::endl;

    return 0;
}