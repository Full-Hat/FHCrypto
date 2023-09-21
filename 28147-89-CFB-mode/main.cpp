#include <bitset>
#include <iostream>

#include "file.h"

#include "CFB.h"

int main(int argc, char** argv)
{
    auto key1 = GOST_28147_89::SplitKey({"0123456789012345678901234567890"});
    std::array<uint32_t, 8> reversed_key;
    std::reverse_copy(key1.begin(), key1.end(), reversed_key.begin());

    auto result = GOST_28147_89::FeistelNetEncrypt(15, key1);
    auto decrypted = GOST_28147_89::FeistelNetDecrypt(result, reversed_key);

    std::cout << decrypted;

    return 0;
}