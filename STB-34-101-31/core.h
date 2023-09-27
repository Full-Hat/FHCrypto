#include <_types/_uint32_t.h>
#include <array>
#include <cstddef>
#include <tuple>

#include "consts.h"

inline std::array<uint32_t, 8> SplitKey(const std::array<char, 32> &_key)
{
    std::array<uint32_t, 8> sub_keys;
    
    for (int i = 0; i < 8; ++i)
    {
        sub_keys[i] = uint32_t(_key[i*4+0]) | 
                    uint32_t(_key[i*4+1]) << 8 |
                    uint32_t(_key[i*4+2]) << 16 |
                    uint32_t(_key[i*4+3]) << 24;
    }

    return sub_keys;
}

inline uint32_t gPermutate(uint32_t w)
{
    return uint32_t(p_table[w&0xFF]) | uint32_t(p_table[w>>8&0xFF])<<8 |
            uint32_t(p_table[w>>16&0xFF])<<16 | uint32_t(p_table[w>>24])<<24;
}

template <size_t offset>
uint32_t gTransform(uint32_t value)
{
    auto permutated_value = gPermutate(value);
    return permutated_value<<offset | permutated_value>>(32-offset);
}

using abcd_t = std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>;
using sub_keys_t = std::array<uint32_t, 8>;

inline abcd_t EncryptBlock(abcd_t abcd, sub_keys_t sub_keys)
{
    for (int i = 0; i < 8; ++i)
    {
        auto [a, b, c, d] = abcd;
        
        b ^= gTransform<5>(a + sub_keys[(8 - i) & 7]);
        c ^= gTransform<21>(d + sub_keys[(9 - i) & 7]);
        a -= gTransform<13>(b + sub_keys[(10 - i) & 7]);

        const auto e = gTransform<21>(b+c+sub_keys[(11-i)&7]) + uint32_t(i) + 1;
		b += e;
		c -= e;

		d += gTransform<13>(c + sub_keys[(12-i)&7]);
		b ^= gTransform<21>(a + sub_keys[(13-i)&7]);
		c ^= gTransform<5>(d + sub_keys[(14-i)&7]);

        abcd = { b, d, a, c };
    }

    auto [a, b, c, d] = abcd;

    return { b, d, a, c };
}

inline abcd_t DecryptBlock(abcd_t abcd, sub_keys_t sub_keys)
{
    for (int i = 7; i >= 0; --i)
    {
        auto [a, b, c, d] = abcd;
        
        b ^= gTransform<5>(a + sub_keys[(14 - i) & 7]);
        c ^= gTransform<21>(d + sub_keys[(13 - i) & 7]);
        a -= gTransform<13>(b + sub_keys[(12 - i) & 7]);

        const auto e = gTransform<21>(b+c+sub_keys[(11 - i) & 7]) + uint32_t(i) + 1;
		b += e;
		c -= e;

		d += gTransform<13>(c + sub_keys[(10 - i) & 7]);
		b ^= gTransform<21>(a + sub_keys[(9 - i) & 7]);
		c ^= gTransform<5>(d + sub_keys[(8 - i) & 7]);

        abcd = { b, d, a, c };
    }

    auto [a, b, c, d] = abcd;

    return { b, d, a, c };
}


