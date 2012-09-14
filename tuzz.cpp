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

template<typename InIt, typename OutIt>
std::vector<tuzz::finjector_t<InIt, OutIt>> make_finjectors() {
  using namespace tuzz;

  return {
    make_rep_n_finjector<InIt, OutIt>(1, position::begining),
    make_rep_n_finjector<InIt, OutIt>(5, position::end),
    make_rep_n_finjector<InIt, OutIt>
      (3, position::begining | position::middle | position::end),
    make_rep_n_char_finjector<InIt, OutIt>(1, '|', position::end, false),
    make_transform_finjector<InIt, OutIt>(::toupper),
    make_transform_finjector<InIt, OutIt>(::tolower),
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
    find_sep<initializer_list<char>>(str.cbegin(), str.cend(),
             { ',', ';', ':', '/', '(', ')', '-', '\n' });

  // Cretate the fault injectors to use
  auto finjectors = make_finjectors<std::string::const_iterator,
                                    back_insert_iterator<string>>();

  cout << endl;
  cout << "Found: " << sep_iters.size() << " separators" << endl;
  cout << "Loaded: " << finjectors.size() << " finjectors" << endl;
  cout << endl;

  // Run each fault injector over every chunk
  auto osit = ostream_iterator<char>(cout);
  size_t variant = 0;
  for (auto finjector : finjectors) {
    printf("------------- file %04d -----------------\n", variant);
    string out_str;
    apply_finjector(str.cbegin(), str.cend(),
                    back_inserter(out_str),
                    sep_iters, finjector);
    copy(out_str.cbegin(), out_str.cend(), osit);
    cout << "-----------------------------------------" << endl;

    ++variant;
  }

  return 0;
}
