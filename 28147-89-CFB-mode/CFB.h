#include "28147-89.h"
#include "file.h"
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cstring>
#include <vector>

class GOST_28147_89_CFB : GOST_28147_89
{
public:
    static uint64_t Iteration(uint64_t _gamma, uint64_t _plain_text, std::array<uint32_t, 8> &_sub_keys)
    {
        const uint64_t encrypted_gamma = GOST_28147_89::FeistelNetDecrypt(_gamma, _sub_keys);

        return _plain_text ^ encrypted_gamma;
    }

    static void test()
    {
        auto file = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/text.txt");

        uint64_t init_gamma = 0x11112234AAAA1113;

        std::vector<uint64_t> text;

        for (auto s : file.ReadBlock().first)
        {
            std::cout << std::bitset<64>(s) << std::endl;
            auto gamma = FeistelNetEncrypt(s, SplitKey({"0123456789012345678901234567890"}));

            //init_gamma = s ^ gamma;

            text.push_back(gamma);
        }

        WriteToFile(text, "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/encrypted.txt");
    }

    static void untest()
    {
        auto file = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/encrypted.txt");

        uint64_t init_gamma = 0x11112234AAAA1113;

        std::vector<uint64_t> text;

        for (auto s : file.ReadBlock().first)
        {
            auto key1 = SplitKey({"0123456789012345678901234567890"});
            std::array<uint32_t, 8> key;
            std::reverse_copy(key1.begin(), key1.end(), key.begin());
            auto gamma = FeistelNetDecrypt(s, key);
            std::cout << std::bitset<64>(s) << std::endl;
            //init_gamma = s ^ gamma;

            text.push_back(gamma);
        }

        WriteToFile(text, "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/decrypted.txt");
    }
};