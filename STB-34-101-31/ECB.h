#include "core.h"
#include "consts.h"

#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <functional>
#include <span>
#include <vector>

using data_t = std::vector<uint8_t>;
using convert_func_t = std::function<abcd_t(abcd_t, sub_keys_t)>;


inline data_t CryptECB(const data_t data, sub_keys_t sub_keys, const convert_func_t &convert_func)
{
    auto bc = data.size() / block_size;
    auto last_block_size = data.size() & (block_size - 1);

    if (last_block_size != 0)
    {
        --bc;
    }

    data_t buff(buff_size);

    uint32_t s0, s1, s2, s3;

    for (std::span<uint8_t> sl {buff.begin(), buff.end()}; 
        sl.size() != 0 && bc != 0; 
        sl = sl.subspan(block_size, sl.size() - block_size), --bc)
    {
        //s = mem; (s = Xi)
        s0 = uint32_t(sl[0]) | uint32_t(sl[1])<<8 | uint32_t(sl[2])<<16 | uint32_t(sl[3])<<24;
        s1 = uint32_t(sl[4]) | uint32_t(sl[5])<<8 | uint32_t(sl[6])<<16 | uint32_t(sl[7])<<24;
        s2 = uint32_t(sl[8]) | uint32_t(sl[9])<<8 | uint32_t(sl[10])<<16 | uint32_t(sl[11])<<24;
        s3 = uint32_t(sl[12]) | uint32_t(sl[13])<<8 | uint32_t(sl[14])<<16 | uint32_t(sl[15])<<24;

        //s = F(s); (s = Yi)
        std::tie(s0, s1, s2, s3) = convert_func({s0, s1, s2, s3}, sub_keys);

        //mem = s; (mem = Yi)
        sl[0] = uint8_t(s0);
        sl[1] = uint8_t(s0 >> 8);
        sl[2] = uint8_t(s0 >> 16);
        sl[3] = uint8_t(s0 >> 24);
        sl[4] = uint8_t(s1);
        sl[5] = uint8_t(s1 >> 8);
        sl[6] = uint8_t(s1 >> 16);
        sl[7] = uint8_t(s1 >> 24);
        sl[8] = uint8_t(s2);
        sl[9] = uint8_t(s2 >> 8);
        sl[10] = uint8_t(s2 >> 16);
        sl[11] = uint8_t(s2 >> 24);
        sl[12] = uint8_t(s3);
        sl[13] = uint8_t(s3 >> 8);
        sl[14] = uint8_t(s3 >> 16);
        sl[15] = uint8_t(s3 >> 24);
    }

    data_t crypted_data {
        buff.begin(), buff.end()
    };

    if (last_block_size != 0)
    {
        //data_t readed_data { data.begin() + }
    }

    return crypted_data;
}

