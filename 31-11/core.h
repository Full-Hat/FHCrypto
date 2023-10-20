#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <stdexcept>
#include <cassert>

#include "translations.h"
#include "numbers_sums.h"

using arr_8_64 = std::array<uint8_t, 64>;
using arr_8_32 = std::array<uint8_t, 32>;

class Hash
{
public:
  enum class Mode
  {
      MODE_256 = 256,
      MODE_512 = 512,
  };

  static void GetHash(std::istream& ifile, std::ostream& ofile, Mode mode) {
    ifile.seekg(0, std::ios::beg);
    ofile.seekp(0, std::ios::beg);

    int fstBlockSize, fullBlocksNum;

    ifile.seekg(0, std::ios::end);
    size_t size = ifile.tellg();
    ifile.seekg(0, std::ios::beg);
    fstBlockSize = size % 64;
    fullBlocksNum = size / 64;

    arr_8_64 buff { 0 };  // for storing current block

    /* Stage 1 */

    arr_8_64 h;
    if (mode == Mode::MODE_256) {
      std::ranges::fill(h, 1);
    } else if (mode == Mode::MODE_512) {
      std::ranges::fill(h, 0);
    }

    arr_8_64 N { 0 };  // filled with zeros
    arr_8_64 sum { 0 };

    /* Stage 2 */

    for (int i = 1; i <= fullBlocksNum; i++) {
      ifile.seekg(-64 * i, std::ios::end);
      ifile.read(reinterpret_cast<char*>(buff.data()), 64);

      h = Translations::tr_g(N, h, buff);
      N = addBigSmall(N, 512);
      sum = addBigBig(sum, buff);
    }

    /* Stage 3 */

    if (fstBlockSize != 0) {
      ifile.seekg(0, std::ios::beg);
      ifile.read(reinterpret_cast<char*>(buff.data()) + 64 - fstBlockSize,
                fstBlockSize);
    }

    for (int i = 0; i < 64 - fstBlockSize; i++) {
      buff[i] = 0;
    }
    buff[64 - fstBlockSize - 1] = 1;

    h = Translations::tr_g(N, h, buff);
    N = addBigSmall(N, uint64_t(fstBlockSize * 8));
    sum = addBigBig(sum, buff);

    h = Translations::tr_g(arr_8_64({0}), h, N);
    h = Translations::tr_g(arr_8_64({0}), h, sum);

    switch (mode)
    {
      case Mode::MODE_256:
      {
        arr_8_32 result;
        std::copy(h.begin(), h.begin() + 32, result.begin());
        ofile.write(reinterpret_cast<const char*>(result.data()), result.size());
        break;
      }
      case Mode::MODE_512:
      {
        assert(h.size() == static_cast<unsigned int>(mode) / 8);
        ofile.write(reinterpret_cast<const char*>(h.data()), h.size());
        break;
      }
      default:
          throw std::runtime_error("Not implemented");
    }
  }
};
