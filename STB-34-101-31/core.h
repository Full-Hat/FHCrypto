#pragma once

#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <array>
#include <cstddef>
#include <tuple>

#include "consts.h"

class STB_34_101_31
{
    static std::array<uint32_t, 8> SplitKey(const std::array<uint8_t, 32> &_key)
    {
        std::array<uint32_t, 8> m_sub_keys;
        
        for (int i = 0; i < 8; ++i)
        {
            m_sub_keys[i] = uint32_t(_key[i*4+0]) | 
                        uint32_t(_key[i*4+1]) << 8 |
                        uint32_t(_key[i*4+2]) << 16 |
                        uint32_t(_key[i*4+3]) << 24;
        }

        return m_sub_keys;
    }

    static uint32_t gPermutate(uint32_t w)
    {
        return uint32_t(p_table[w&0xFF]) | uint32_t(p_table[w>>8&0xFF])<<8 |
                uint32_t(p_table[w>>16&0xFF])<<16 | uint32_t(p_table[w>>24])<<24;
    }

    template <size_t offset>
    static uint32_t gTransform(uint32_t value)
    {
        auto permutated_value = gPermutate(value);
        return permutated_value<<offset | permutated_value>>(32-offset);
    }

public:
    using abcd_t = std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>;
    using key_t = std::array<uint8_t, 32>;
    using sub_keys_t = std::array<uint32_t, 8>;

    STB_34_101_31(const STB_34_101_31::key_t key)
    {
        m_sub_keys = SplitKey(key);
    }
    

    STB_34_101_31::abcd_t EncryptBlock(STB_34_101_31::abcd_t abcd)
    {
        for (int i = 0; i < 8; ++i)
        {
            auto [a, b, c, d] = abcd;
            
            b ^= gTransform<5>(a + m_sub_keys[(8 - i) & 7]);
            c ^= gTransform<21>(d + m_sub_keys[(9 - i) & 7]);
            a -= gTransform<13>(b + m_sub_keys[(10 - i) & 7]);

            const auto e = gTransform<21>(b+c+m_sub_keys[(11-i)&7]) ^ (uint32_t(i) + 1);
            b += e;
            c -= e;

            d += gTransform<13>(c + m_sub_keys[(12-i)&7]);
            b ^= gTransform<21>(a + m_sub_keys[(13-i)&7]);
            c ^= gTransform<5>(d + m_sub_keys[(14-i)&7]);

            abcd = { b, d, a, c };
        }

        auto [a, b, c, d] = abcd;

        return { b, d, a, c };
    }

    STB_34_101_31::abcd_t DecryptBlock(STB_34_101_31::abcd_t abcd)
    {
        for (int i = 7; i >= 0; --i)
        {
            auto [a, b, c, d] = abcd;
            
            b ^= gTransform<5>(a + m_sub_keys[(14 - i) & 7]);
            c ^= gTransform<21>(d + m_sub_keys[(13 - i) & 7]);
            a -= gTransform<13>(b + m_sub_keys[(12 - i) & 7]);

            const auto e = gTransform<21>(b+c+m_sub_keys[(11 - i) & 7]) ^ (uint32_t(i) + 1);
            b += e;
            c -= e;

            d += gTransform<13>(c + m_sub_keys[(10 - i) & 7]);
            b ^= gTransform<21>(a + m_sub_keys[(9 - i) & 7]);
            c ^= gTransform<5>(d + m_sub_keys[(8 - i) & 7]);

            abcd = { c, a, d, b };
        }

        auto [a, b, c, d] = abcd;

        return { c, a, d, b };
    }

protected:
    sub_keys_t m_sub_keys;
};
