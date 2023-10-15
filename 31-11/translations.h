#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>

#include "consts.h"

std::vector<uint8_t> xor_func(const std::vector<uint8_t>& p1,
                              const std::vector<uint8_t>& p2) {
  std::vector<uint8_t> result(64);
  for (int i = 0; i < 64; i++) {
    result[i] = p1[i] ^ p2[i];
  }
  return result;
}

std::vector<uint8_t> trX(const std::vector<uint8_t>& k,
                         const std::vector<uint8_t>& a) {
  return xor_func(k, a);
}

std::vector<uint8_t> trS(const std::vector<uint8_t>& a) {
  std::vector<uint8_t> result(64);
  for (int i = 0; i < 64; i++) {
    result[i] = pi_subst[a[i]];
  }
  return result;
}

std::vector<uint8_t> trP(const std::vector<uint8_t>& a) {
  std::vector<uint8_t> result(64);
  for (int i = 0; i < 64; i++) {
    result[i] = a[tau_subst[i]];
  }
  return result;
}

std::vector<uint8_t> trL(const std::vector<uint8_t>& a) {
  std::vector<uint8_t> result(64);

  uint64_t n, accum;

  for (int i = 0; i < 8; i++) {
    n = *reinterpret_cast<const uint64_t*>(&a[8 * i]);
    accum = 0;

    for (int j = 0; j < 64; j++) {
      accum ^= ((n >> j) & 1) * mtx_A[63 - j];
    }

    *reinterpret_cast<uint64_t*>(&result[8 * i]) = accum;
  }

  return result;
}

std::vector<uint8_t> trE(std::vector<uint8_t> K,
                         const std::vector<uint8_t>& m) {
  std::vector<uint8_t> result(64);
  std::copy(m.begin(), m.end(), result.begin());

  for (int i = 0; i < 12; i++) {
    result = trX(K, result);
    result = trS(result);
    result = trP(result);
    result = trL(result);

    K = xor_func(K, std::vector<uint8_t>(C[i], C[i] + 64));
    K = trS(K);
    K = trP(K);
    K = trL(K);
  }

  result = trX(K, result);
  return result;
}

std::vector<uint8_t> tr_g(const std::vector<uint8_t>& N,
                          const std::vector<uint8_t>& h,
                          const std::vector<uint8_t>& m) {
  std::vector<uint8_t> result = xor_func(h, N);
  result = trS(result);
  result = trP(result);
  result = trL(result);
  result = trE(result, m);
  result = xor_func(result, h);
  result = xor_func(result, m);

  return result;
}
