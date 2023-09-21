#include <_types/_uint32_t.h>
#include <_types/_uint64_t.h>
#include <array>
#include <cassert>
#include <iostream>
#include <sys/_types/_u_char.h>
#include <sys/types.h>
#include <variant>
#include <vector>

#include "file.h"
#include "consts.h"

class GOST_28147_89
{
public:
    static std::array<uint32_t, 8> SplitKey(const std::array<char, 32> &_key)
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
    static uint32_t F(uint32_t _block_half, uint32_t _sub_key)
    {
        auto result = _block_half + _sub_key;

        result = uint32_t(sbox[0][result&0xF]) |
                uint32_t(sbox[1][result>>4&0xF])<<4 |
                uint32_t(sbox[2][result>>8&0xF])<<8 |
                uint32_t(sbox[3][result>>12&0xF])<<12 |
                uint32_t(sbox[4][result>>16&0xF])<<16 |
                uint32_t(sbox[5][result>>20&0xF])<<20 |
                uint32_t(sbox[6][result>>24&0xF])<<24 |
                uint32_t(sbox[7][result>>28&0xF])<<28;

        return result<<11 | result>>(32-11);
    }

    static uint64_t FeistelNetEncrypt(uint64_t _lr, const std::array<uint32_t, 8> &_sub_keys)
    {
        // l is now lower part of lr
        uint32_t l = (uint32_t)_lr;
        uint32_t r = (uint32_t)(_lr >> 32);

        for (int i = 0; i < 24; ++i)
        {
            l = r;
            r = l ^ F(r, _sub_keys[i&7]);
        }
        for (int i = 24; i < 32; ++i)
        {
            l = r;
            r = l ^ F(r, _sub_keys[7 ^ i&7]);
        }

        return ((uint64_t)l << 32) | (uint64_t)r;
    }

    static uint64_t FeistelNetDecrypt(uint64_t _lr, const std::array<uint32_t, 8> &_sub_keys)
    {
        // l is now lower part of lr
        uint32_t l = (uint32_t)_lr;
        uint32_t r = (uint32_t)(_lr >> 32);

        for (int i = 31; i >= 24; --i)
        {
            l = r;
            r = l ^ F(r, _sub_keys[7 ^ i&7]);
        }
        for (int i = 23; i >= 0; --i)
        {
            l = r;
            r = l ^ F(r, _sub_keys[i&7]);
        }

        return ((uint64_t)l << 32) | (uint64_t)r;
    }
};
