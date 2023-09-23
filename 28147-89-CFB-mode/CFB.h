#include "28147-89.h"
#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstring>
#include <utility>
#include <vector>

class GOST_28147_89_CFB : GOST_28147_89
{
public:
    GOST_28147_89_CFB(const std::array<char, 32> &_key)
    {
        m_sub_keys = SplitKey(_key);
    }

    std::vector<uint64_t> Encrypt(const std::vector<uint64_t> &_plain_text, uint64_t _gamma) const
    {
        std::vector<uint64_t> crypted_text(_plain_text.size());

        for (int i = 0; i < _plain_text.size(); ++i)
        {
            _gamma = CryptText(_plain_text[i], _gamma);
            crypted_text[i] = _gamma;
        }

        return crypted_text;
    }

    std::vector<uint64_t> Decrypt(const std::vector<uint64_t> &_plain_text, uint64_t _gamma) const
    {
        std::vector<uint64_t> decrypted_text(_plain_text.size());

        for (int i = 0; i < _plain_text.size(); ++i)
        {
            auto next_gamma = _plain_text[i];
            decrypted_text[i] = CryptText(_plain_text[i], _gamma);
            _gamma = next_gamma;
        }

        return decrypted_text;
    }

protected:
    uint64_t CryptText(uint64_t _plain_text, uint64_t _gamma) const
    {
        return GOST_28147_89::FeistelNetEncrypt(_gamma, m_sub_keys) ^ _plain_text;
    }

    std::array<uint32_t, 8> m_sub_keys;
};

inline void test_CFB()
{
    std::array<char, 32> key { "MyVeryGoodKeyForEncryptionSomeS" };

    GOST_28147_89_CFB crypter(key);
    std::vector<uint64_t> data { 1, 1, 110 };
    auto crypted = crypter.Encrypt(data, 0);
    auto decrypted = crypter.Decrypt(crypted, 0);

    for (int i = 0; i < crypted.size(); ++i)
    {
        assert(data[i] == decrypted[i]);
    }
}