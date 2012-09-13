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

#include "tuzz.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <iterator>

const std::string read_file(const char* fn) {
  using sbuf_it_t = std::istreambuf_iterator<char>;
  std::ifstream fs(fn);
  return std::string((sbuf_it_t(fs)), sbuf_it_t());
}

using cstr_it = std::string::const_iterator;

std::vector<cstr_it> find_sep(cstr_it first, cstr_it end, std::initializer_list<char> sep) {
  std::vector<cstr_it> sep_iters;

  for (auto it = first; it != end;) {
    it = find_if(it, end,
      [&] (char c) { return std::find(sep.begin(), sep.end(), c) != sep.end(); }
		);

    if (it != end)
      sep_iters.push_back(it++);
  }

  return sep_iters;
}

template<typename InIt, typename OutIt>
std::vector<tuzz::finjector_t<InIt, OutIt>> make_finjectors() {
  using namespace tuzz;

  return {
    [] (InIt begin, InIt end, OutIt out) {
      return rep_n(begin, end, out, 3, position::begining);
    },
    [] (InIt begin, InIt end, OutIt out) {
      return rep_n(begin, end, out, 3, position::end);
    },
    [] (InIt begin, InIt end, OutIt out) {
      return rep_n(begin, end, out, 1,
        position::begining | position::middle | position::end);
    },
    [] (InIt begin, InIt end, OutIt out) {
      return rep_n(begin, end, out, 2, position::middle, '|', false);
    },
    [] (InIt begin, InIt end, OutIt out) {
      return transform(begin, end, out, ::toupper);
    },
    [] (InIt begin, InIt end, OutIt out) {
      return transform(begin, end, out, ::tolower);
    },
  };
}


int main(int argc, char* argv[]) {
  using namespace std;
  using namespace tuzz;

  // Load a pristine text file
  const string str = read_file(argv[1]);

  cout << "------------- original file -------------" << endl;
  cout << str << endl;
  cout << "-----------------------------------------" << endl;

  // Find chunks
  auto sep_iters =
    find_sep(str.cbegin(), str.cend(),
             { ',', ';', ':', '/', '(', ')', '-', '\n' });

  // Cretate the fault injectors to use
  auto finjectors = make_finjectors<cstr_it, ostream_iterator<char>>();

  cout << endl;
  cout << "Found: " << sep_iters.size() << " separators" << endl;
  cout << "Loaded: " << finjectors.size() << " finjectors" << endl;
  cout << endl;

  // Run each fault injector over every chunk
  auto osit = ostream_iterator<char>(cout);
  size_t variant = 0;
  for (auto finjector : finjectors) {
    printf("------------- file %04d -----------------\n", variant);
    auto str_it = str.cbegin();
    for (auto part_it : sep_iters) {
      finjector(str_it, part_it, osit);
      *osit = *part_it;
      ++osit;
      str_it = part_it + 1;
    }
    cout << "-----------------------------------------" << endl;

    ++variant;
  }

  return 0;
}
