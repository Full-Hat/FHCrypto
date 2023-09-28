#include <vector>
#include <span>

#include "consts.h"
#include "core.h"

class STB_34_101_31_CTR : STB_34_101_31
{
public:
    using data_t = std::vector<uint8_t>;
    
    STB_34_101_31_CTR(STB_34_101_31::key_t key) : STB_34_101_31(key)
    {
    }

    data_t Encrypt(data_t data)
    {
        auto [ s0, s1, s2, s3 ] = EncryptBlock({ IV[0], IV[1], IV[2], IV[3] });
        
        auto drop = (block_size - data.size() & (block_size - 1)) & (block_size - 1);
        data.resize(data.size() + drop);
        
        std::span<uint8_t> sl = data;
        for (;
            sl.size() != 0;
            sl = sl.subspan(block_size))
        {
            if (s0++; s0 == 0)
            {
                if (s1++; s1 == 0)
                {
                    if (s2++; s2 == 0)
                    {
                        ++s3;
                    }
                }
            }

            auto [ se0, se1, se2, se3 ] = EncryptBlock({ s0, s1, s2, s3 });

            sl[0] ^= uint8_t(se0);
            sl[1] ^= uint8_t(se0 >> 8);
            sl[2] ^= uint8_t(se0 >> 16);
            sl[3] ^= uint8_t(se0 >> 24);
            sl[4] ^= uint8_t(se1);
            sl[5] ^= uint8_t(se1 >> 8);
            sl[6] ^= uint8_t(se1 >> 16);
            sl[7] ^= uint8_t(se1 >> 24);
            sl[8] ^= uint8_t(se2);
            sl[9] ^= uint8_t(se2 >> 8);
            sl[10] ^= uint8_t(se2 >> 16);
            sl[11] ^= uint8_t(se2 >> 24);
            sl[12] ^= uint8_t(se3);
            sl[13] ^= uint8_t(se3 >> 8);
            sl[14] ^= uint8_t(se3 >> 16);
            sl[15] ^= uint8_t(se3 >> 24);
        }
        
        data.resize(data.size() - drop);
        return data;
    }

    data_t Decrypt(data_t data)
    {
        return Encrypt(data);
    }
};
