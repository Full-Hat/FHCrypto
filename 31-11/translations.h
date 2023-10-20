#pragma once

#include <algorithm>
#include <span>
#include <ranges>

#include "consts.h"
#include "numbers_sums.h"

template<typename T>
concept byte_span_const = requires(T a) {
    { a.data() } -> std::same_as<const uint8_t*>;
    { a.size() } -> std::same_as<size_t>;
};

template<typename T>
concept byte_span_non_const = requires(T a) {
    { a.data() } -> std::same_as<uint8_t*>;
    { a.size() } -> std::same_as<size_t>;
};

template<typename T>
concept byte_span = byte_span_const<T> || byte_span_non_const<T>;

class Translations
{
public:
  template<byte_span Span1, byte_span Span2>
  static arr_8_64 xor_func(const Span1& p1,
                                Span2& p2) {
    arr_8_64 result;
    for (int i = 0; i < 64; i++) {
      result[i] = p1[i] ^ p2[i];
    }
    return result;
  }

  static arr_8_64 trX(const arr_8_64& k,
                          const arr_8_64& a) {
    std::span<const uint8_t> k_span{ k };
    std::span<const uint8_t> a_span{ a };
    return xor_func(k_span, a_span);
  }

  static arr_8_64 trS(const arr_8_64& a) {
    arr_8_64 result;
    for (int i = 0; i < 64; i++) {
      result[i] = pi_subst[a[i]];
    }
    return result;
  }

  static arr_8_64 trP(const arr_8_64& a) {
    arr_8_64 result;
    for (int i = 0; i < 64; i++) {
      result[i] = a[tau_subst[i]];
    }
    return result;
  }

  static uint64_t Uint64(std::span<uint8_t>& buf) {
      uint64_t number = 0;
      for (int i = 0; i < 8; ++i) {
          number = (number << 8) | buf[i];
      }
      return number;
  }

  static void PutUint64(uint64_t number, std::span<uint8_t> buf) {
      for (int i = 7; i >= 0; --i) {
          buf[i] = number & 0xFF;
          number >>= 8;
      }
  }

  static arr_8_64 trL(arr_8_64& a) {
      arr_8_64 result{0};

      uint64_t n = 0;

      for (int i = 0; i < 8; ++i)
      {
        std::span<uint8_t> a_span(a.begin() + 8 * i, a.end());
        n = Uint64(a_span);
        uint64_t accum = 0;
        for (int j = 0; j < 64; ++j)
        {
          accum ^= ((n >> j) & 1) * mtx_A[63 - j];
        }

        PutUint64(accum, std::span<uint8_t>(result.begin() + 8 * i, result.end()));
      }

      return result;
  }

  static arr_8_64 trE(arr_8_64 K,
                          const arr_8_64& m) {
    arr_8_64 result{0};
    std::copy(m.begin(), m.end(), result.begin());

    for (int i = 0; i < 12; i++) {
      result = trX(K, result);
      result = trS(result);
      result = trP(result);
      result = trL(result);

      std::span<const uint8_t> C_block { C[i], C[i] + 64 };
      K = xor_func(K, C_block);
      K = trS(K);
      K = trP(K);
      K = trL(K);
    }

    result = trX(K, result);
    return result;
  }

  static arr_8_64 tr_g(const arr_8_64& N,
                            const arr_8_64& h,
                            const arr_8_64& m) {
    arr_8_64 result = xor_func(h, N);
    result = trS(result);
    result = trP(result);
    result = trL(result);
    result = trE(result, m);
    result = xor_func(result, h);
    result = xor_func(result, m);

    return result;
  }
};
