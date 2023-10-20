#include <fstream>

#include "core.h"

int main() {
  std::cout << "Hello world";
  std::ofstream fout("/Users/full-hat/Documents/MyProjects/FHCrypto/31-11/output.txt");
  std::ifstream fin("/Users/full-hat/Documents/MyProjects/FHCrypto/31-11/input.txt");
  if (!fout.is_open() || !fin.is_open()) {
    throw std::runtime_error("Can't open files");
  }
  Hash::GetHash(fin, fout, Hash::Mode::MODE_512);
}
