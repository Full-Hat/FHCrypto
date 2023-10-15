#include <iostream>

#include "core.h"

int main() {
  std::cout << "Hello world";
  std::ofstream fout("D:\\EpicGames\\Projects\\FHCrypto\\31-11\\output.txt");
  std::ifstream fin("D:\\EpicGames\\Projects\\FHCrypto\\31-11\\input.txt");
  if (!fout.is_open() || !fin.is_open()) {
    throw std::runtime_error("Can't open files");
  }
  Hash(fin, fout, Mode::MODE_256);
}
