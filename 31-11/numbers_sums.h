#pragma once

#include <cstdint>
#include <vector>

std::vector<uint8_t> addBigSmall(const std::vector<uint8_t>& b, uint64_t a) {
  std::vector<uint64_t> sum(8);
  std::vector<uint8_t> result(64);

  // unpacking b into vector of uint64_t's
  for (int i = 0; i < 8; i++) {
    sum[i] = *reinterpret_cast<const uint64_t*>(&b[8 * i]);
  }

  // performing addition (with probably carry)
  uint64_t old = sum[7];
  sum[7] += a;
  if (old > sum[7]) {
    int ind = 7;
    while (true) {
      ind--;
      sum[ind]++;
      if (sum[ind] != 0) {
        break;
      }
    }
  }

  // writing result
  for (int i = 0; i < 8; i++) {
    *reinterpret_cast<uint64_t*>(&result[8 * i]) = sum[i];
  }

  return result;
}

std::vector<uint8_t> addBigBig(const std::vector<uint8_t>& p1,
                               const std::vector<uint8_t>& p2) {
  std::vector<uint64_t> num1(8);
  std::vector<uint64_t> num2(8);
  std::vector<uint8_t> result(64);

  // retrieving params as sets of subsequent 8-bit unsigned int values
  for (int i = 0; i < 8; i++) {
    num1[i] = *reinterpret_cast<const uint64_t*>(&p1[8 * i]);
    num2[i] = *reinterpret_cast<const uint64_t*>(&p2[8 * i]);
  }

  uint64_t carry = 0;
  // calculating (writing sum into num1, checking overflow using num2)
  for (int i = 7; i >= 0; i--) {
    uint64_t old_num1_i = num1[i];
    num1[i] += num2[i] + carry;
    if (num1[i] < old_num1_i || (num1[i] == old_num1_i && carry == 1)) {
      carry = 1;
    } else {
      carry = 0;
    }
  }

  // writing result
  for (int i = 0; i < 8; i++) {
    *reinterpret_cast<uint64_t*>(&result[8 * i]) = num1[i];
  }

  return result;
}
