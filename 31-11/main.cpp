#include <cstddef>
#include <fstream>
#include <stdexcept>

#include "core.h"

int main(int argc, char *argv[]) {
  auto mode = std::string(argv[1]) == "512" ? Hash::Mode::MODE_512 : Hash::Mode::MODE_256;
  
  std::ifstream fin(argv[2]);
  std::ofstream fout(argv[3]);
  if (!fout.is_open() || !fin.is_open()) {
    throw std::runtime_error("Can't open files");
  }
  Hash::GetHash(fin, fout, mode);
}
