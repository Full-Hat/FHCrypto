#include <vector>
#include <span>

#include "consts.h"
#include "core.h"


#include <iostream>

class STB_34_101_31_CFB : protected STB_34_101_31
{
public:
    using data_t = std::vector<uint8_t>;

    STB_34_101_31_CFB(STB_34_101_31::key_t key) : STB_34_101_31(key)
    {
    }

    data_t EncryptCFB(data_t data)
    {
        auto [s0, s1, s2, s3] = EncryptBlock({IV[0], IV[1], IV[2], IV[3]});

        auto drop = (block_size - data.size() & (block_size - 1)) & (block_size - 1);
        data.resize(data.size() + drop);


        std::span<uint8_t> sl = data;
        for (;
            sl.size() != 0;
            sl = sl.subspan(block_size))
        {
        //std::ranges::for_each(sl, [](auto value) { std::cout << value << " "; });
            std::tie(s0, s1, s2, s3) = EncryptBlock({ s0, s1, s2, s3 });

            sl[0] ^= uint8_t(s0);
            sl[1] ^= uint8_t(s0 >> 8);
            sl[2] ^= uint8_t(s0 >> 16);
            sl[3] ^= uint8_t(s0 >> 24);
            sl[4] ^= uint8_t(s1);
            sl[5] ^= uint8_t(s1 >> 8);
            sl[6] ^= uint8_t(s1 >> 16);
            sl[7] ^= uint8_t(s1 >> 24);
            sl[8] ^= uint8_t(s2);
            sl[9] ^= uint8_t(s2 >> 8);
            sl[10] ^= uint8_t(s2 >> 16);
            sl[11] ^= uint8_t(s2 >> 24);
            sl[12] ^= uint8_t(s3);
            sl[13] ^= uint8_t(s3 >> 8);
            sl[14] ^= uint8_t(s3 >> 16);
            sl[15] ^= uint8_t(s3 >> 24);

            s0 = uint32_t(sl[0]) | uint32_t(sl[1])<<8 | uint32_t(sl[2])<<16 | uint32_t(sl[3])<<24;
            s1 = uint32_t(sl[4]) | uint32_t(sl[5])<<8 | uint32_t(sl[6])<<16 | uint32_t(sl[7])<<24;
            s2 = uint32_t(sl[8]) | uint32_t(sl[9])<<8 | uint32_t(sl[10])<<16 | uint32_t(sl[11])<<24;
            s3 = uint32_t(sl[12]) | uint32_t(sl[13])<<8 | uint32_t(sl[14])<<16 | uint32_t(sl[15])<<24;
        }

        data.resize(data.size() - drop);
        return data;
    }

    data_t DecryptCFB(data_t data)
    {
        auto [s0, s1, s2, s3] = EncryptBlock({IV[0], IV[1], IV[2], IV[3]});

        auto drop = (block_size - data.size() & (block_size - 1)) & (block_size - 1);
        data.resize(data.size() + drop);

        for (std::span<uint8_t> sl { data.begin(), data.end() };
            sl.size() != 0;
            sl = sl.subspan(block_size))
        {
            std::tie(s0, s1, s2, s3) = EncryptBlock({ s0, s1, s2, s3 });

            sl[0] ^= uint8_t(s0);
            sl[1] ^= uint8_t(s0 >> 8);
            sl[2] ^= uint8_t(s0 >> 16);
            sl[3] ^= uint8_t(s0 >> 24);
            sl[4] ^= uint8_t(s1);
            sl[5] ^= uint8_t(s1 >> 8);
            sl[6] ^= uint8_t(s1 >> 16);
            sl[7] ^= uint8_t(s1 >> 24);
            sl[8] ^= uint8_t(s2);
            sl[9] ^= uint8_t(s2 >> 8);
            sl[10] ^= uint8_t(s2 >> 16);
            sl[11] ^= uint8_t(s2 >> 24);
            sl[12] ^= uint8_t(s3);
            sl[13] ^= uint8_t(s3 >> 8);
            sl[14] ^= uint8_t(s3 >> 16);
            sl[15] ^= uint8_t(s3 >> 24);

            //i wonder whether this still gets optimized :O
            s0 = (uint32_t(sl[0]) ^ s0&0xFF) | (uint32_t(sl[1])^s0>>8&0xFF)<<8 |
                (uint32_t(sl[2])^s0>>16&0xFF)<<16 | (uint32_t(sl[3])^s0>>24&0xFF)<<24;
            s1 = (uint32_t(sl[4]) ^ s1&0xFF) | (uint32_t(sl[5])^s1>>8&0xFF)<<8 |
                (uint32_t(sl[6])^s1>>16&0xFF)<<16 | (uint32_t(sl[7])^s1>>24&0xFF)<<24;
            s2 = (uint32_t(sl[8]) ^ s2&0xFF) | (uint32_t(sl[9])^s2>>8&0xFF)<<8 |
                (uint32_t(sl[10])^s2>>16&0xFF)<<16 | (uint32_t(sl[11])^s2>>24&0xFF)<<24;
            s3 = (uint32_t(sl[12]) ^ s3&0xFF) | (uint32_t(sl[13])^s3>>8&0xFF)<<8 |
                (uint32_t(sl[14])^s3>>16&0xFF)<<16 | (uint32_t(sl[15])^s3>>24&0xFF)<<24;
        }

        data.resize(data.size() - drop);
        return data;
    }
};
