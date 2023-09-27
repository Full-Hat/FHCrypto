#include <span>
#include <vector>
#include <iostream>

#include "CFB.h"

int main()
{
    std::array<char, 32> key { "MyVeryGoodKeyForEncryptionSomeS" };
    auto sub_keys = SplitKey(key);

    auto encrypted = EncryptCFB({ '1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8' }, sub_keys);
    auto decrypted = DecryptCFB(encrypted, sub_keys);
}