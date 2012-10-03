/**
 * Copyright (C) 2012 Anders Sundman <anders@4zm.org>
 *
 * This file is part of tuzz - the text fuzzer.
 *
 * tuzz is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tuzz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tuzz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <iterator>

typedef std::istreambuf_iterator<char> sbit;

int main(int argc, const char* argv[]) {
  char buf[128];

  if (argc != 2) {
    std::cout << "Copies data from stdin or file to 128 byte buffer" << std::endl;
    std::cout << "Usage: " << argv[0] << " [filename|-]" << std::endl;
    return 0;
  }

  if (argv[1][0] == '-') {
    // Input from stdin
    std::copy(sbit(std::cin), sbit(), buf);
  }
  else {
    // Input from file
    std::ifstream ifs(argv[1]);
    std::copy(sbit(ifs), sbit(), buf);
  }

  return 0;
}
