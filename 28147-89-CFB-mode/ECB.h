#include "28147-89.h"
#include <_types/_uint64_t.h>
#include <cassert>

class GOST_28147_89_ECB : protected GOST_28147_89
{
public:
    GOST_28147_89_ECB(const std::array<char, 32> &_key)
    {
        m_keys = SplitKey(_key);
    }

    uint64_t Encrypt(uint64_t _value) const
    {
        return FeistelNetEncrypt(_value, m_keys);
    }

    uint64_t Decrypt(uint64_t _value) const
    {
        return FeistelNetDecrypt(_value, m_keys);
    }

protected:
    std::array<uint32_t, 8> m_keys;
};

inline void test_ECB()
{
    std::array<char, 32> key { "MyVeryGoodKeyForEncryptionSomeS" };
    GOST_28147_89_ECB cryptor(key);
    auto encrypted = cryptor.Encrypt(1);
    auto decrypted = cryptor.Decrypt(encrypted);

    assert(1 == decrypted);
}