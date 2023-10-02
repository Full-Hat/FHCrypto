#include <chrono>
#include <ios>
#include <iostream>
#include <ratio>
#include <stdexcept>
#include <string>
#include <thread>
#include <ctime>
#include <ranges>

#include "core.h"
#include "big_rand.h"
#include "file.h"

int main()
{
    std::vector<unsigned int> a(30);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    auto r = Rabin();
    auto encrypted = r.encrypt(a);
    auto decrypted = r.decrypt(encrypted);
    return 0;

    //auto file_reader = FileReader(R"(D:\EpicGames\Projects\FHCrypto\rabin\text.txt)");
    //auto data = file_reader.ReadBlock_8();

    //data = { 1, 2, 3, 4 };

    //auto crypter = Rabin();

    //std::vector<unsigned int> encrypted;
    //std::vector<uint32_t> block;
    //for (int i = 0; i < data.size(); i += 4)
    //{
    //    // pack 8_t to 32_t [1 - 4]
    //    uint32_t value = 0;
    //    for (int k = i; k < i + 4 && k < data.size(); ++k)
    //    {
    //        value += data[k];
    //        value <<= 8;
    //    }
    //    value >>= 8;

    //    // check for block size
    //    block.push_back(value);

    //    if (block.size() == 31)
    //    {
    //        auto encrypted_block = crypter.encrypt(block);
    //        for (auto el : encrypted_block)
    //        {
    //            encrypted.push_back(el);
    //        }
    //        block.clear();
    //        continue;
    //    }
    //    else // block is not full
    //    {
    //        continue;
    //    }
    //}

    //if (block.size() != 0)
    //{
    //    auto encrypted_block = crypter.encrypt({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 });
    //    auto decrypted_block = crypter.decrypt(encrypted_block);
    //    for (auto el : encrypted_block)
    //    {
    //        encrypted.push_back(el);
    //    }
    //    block.clear();
    //}


    //auto decrypted = crypter.decrypt(encrypted);

        //WriteToFile(decrypted, R"(D:\EpicGames\Projects\FHCrypto\rabin\decrypted.txt)", size);
}
