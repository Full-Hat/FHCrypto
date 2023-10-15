#pragma once

#include <cstdint>
#include <fstream>
#include <vector>
#include <cassert>

#include "translations.h"
#include "consts.h"
#include "numbers_sums.h"

enum class Mode
{
    MODE_256 = 256,
    MODE_512 = 512,
};

void Hash(std::ifstream& ifile, std::ofstream& ofile, Mode mode) {
  ifile.seekg(0, std::ios::beg);
  ofile.seekp(0, std::ios::beg);

  int fstBlockSize, fullBlocksNum;

  ifile.seekg(0, std::ios::end);
  size_t size = ifile.tellg();
  ifile.seekg(0, std::ios::beg);
  fstBlockSize = size % 64;
  fullBlocksNum = size / 64;

  std::vector<uint8_t> buff(64);  // for storing current block

  /* Stage 1 */

  std::vector<uint8_t> h(64);
  if (mode == Mode::MODE_256) {
    h = IV_256;
  } else if (mode == Mode::MODE_512) {
    h = IV_512;
  }

  std::vector<uint8_t> N(64);  // filled with zeros
  std::vector<uint8_t> sum(64);

  /* Stage 2 */

  for (int i = 1; i <= fullBlocksNum; i++) {
    ifile.seekg(-64 * i, std::ios::end);
    ifile.read(reinterpret_cast<char*>(buff.data()), 64);

    h = tr_g(N, h, buff);
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

  h = tr_g(N, h, buff);
  N = addBigSmall(N, uint64_t(fstBlockSize * 8));
  sum = addBigBig(sum, buff);

  h = tr_g(std::vector<uint8_t>(64), h, N);
  h = tr_g(std::vector<uint8_t>(64), h, sum);

  if (mode == Mode::MODE_256) {
    h.resize(32);
  }

  assert(h.size() == static_cast<unsigned int>(mode) / 8);
  ofile.write(reinterpret_cast<const char*>(h.data()), h.size());
}
