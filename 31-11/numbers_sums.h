#pragma once

#include "core.h"
#include <cstdint>
#include <vector>

#include <vector>
#include <cstdint>
#include <algorithm>
#include <array>

using arr_8_64 = std::array<uint8_t, 64>;

uint64_t toBigEndian(uint64_t value) {
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        result |= ((value >> (8 * i)) & 0xFF) << (8 * (7 - i));
    }
    return result;
}

arr_8_64 addBigSmall(const arr_8_64& b, uint64_t a) {
    std::array<uint64_t, 8> sum;
    arr_8_64 result{0};

    // Unpacking b into array of uint64's
    for (int i = 0; i < 8; ++i) {
        sum[i] = *reinterpret_cast<const uint64_t*>(&b[8*i]);
        sum[i] = toBigEndian(sum[i]); // Convert to big endian
    }

    // Performing addition (with probably carry)
    uint64_t old = sum[7];
    sum[7] += a;
    if (old > sum[7]) {
        int ind = 7;
        while (true) {
            --ind;
            ++sum[ind];
            if (sum[ind] != 0) {
                break;
            }
        }
    }

    // Writing result
    for (int i = 0; i < 8; ++i) {
        uint64_t bigEndianSum = toBigEndian(sum[i]); // Convert back to big endian
        std::memcpy(&result[8*i], &bigEndianSum, sizeof(uint64_t));
    }

    return result;
}

arr_8_64 addBigBig(const arr_8_64& p1, const arr_8_64& p2) {
    std::array<uint64_t, 8> num1;
    std::array<uint64_t, 8> num2;
    arr_8_64 result{0};

    // Retrieving params as sets of subsequent 8-bit unsigned int values
    for (int i = 0; i < 8; ++i) {
        num1[i] = *reinterpret_cast<const uint64_t*>(&p1[8 * i]);
        num1[i] = toBigEndian(num1[i]); // Convert to big endian

        num2[i] = *reinterpret_cast<const uint64_t*>(&p2[8 * i]);
        num2[i] = toBigEndian(num2[i]); // Convert to big endian
    }

    uint64_t carry = 0;
    // Calculating (writing sum into num1, checking overflow using num2)
    for (int i = 7; i >= 0; --i) {
        uint64_t old_num1_i = num1[i];
        num1[i] += num2[i] + carry;
        if (num1[i] < old_num1_i || (num1[i] == old_num1_i && carry == 1)) {
            carry = 1;
        } else {
            carry = 0;
        }
    }

    // Writing result
    for (int i = 0; i < 8; ++i) {
        uint64_t bigEndianNum = toBigEndian(num1[i]); // Convert back to big endian
        std::memcpy(&result[8 * i], &bigEndianNum, sizeof(uint64_t));
    }

    return result;
}
