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
#include "options.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/program_options.hpp>

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

int main(int argc, const char* argv[]) {
  using namespace tuzz;
  namespace fs = boost::filesystem;
  namespace po = boost::program_options;
  using namespace std;

  tuzz_options opt(argc, argv);

  if (opt.stdin_as_input() || opt.stdout_as_output())
  {
    cout << "-o- and -i- is not yet supported" << endl;
    return 1;
  }
  else if (opt.get_input_paths().size() > 1) {
    cout << "multiple inputs not yet supported" << endl;
    return 1;
  }

  fs::path src_path(opt.get_input_paths()[0]);
  fs::path dst_path(opt.get_output_pattern());

  try {
    if (!fs::exists(src_path) || !fs::is_regular_file(src_path)) {
      cout << "No such file: " << src_path << endl;
      return 1;
    }

    if (fs::exists(dst_path)) {
      if (!fs::is_directory(dst_path)) {
        cout << "Destination " << dst_path << " is not a directory: " << endl;
        return 1;
      }
      else if (!fs::is_empty(dst_path)) {
        cout << "Destination directory " << dst_path << " exists and is not empty." << endl;
        return 1;
      }
    }
    else if (!fs::create_directory(dst_path)) {
      cout << "Failed to create dst: "  << dst_path << endl;
      return 1;
    }
  }
  catch (const fs::filesystem_error& ex) {
    cout << ex.what() << endl;
    return 1;
  }

  cout << "[+] Reading input file: " << src_path <<
    " (" << fs::file_size(src_path) << " Bytes)" << endl;

  // Load a pristine text file
  fs::ifstream ifs(src_path);
  using sbuf_it_t = std::istreambuf_iterator<char>;
  const string str = std::string((sbuf_it_t(ifs)), sbuf_it_t());

  // Find chunks
  auto sep_iters =
    find_sep<initializer_list<char>>(str.cbegin(), str.cend(),
             { ',', ';', ':', '/', '(', ')', '-', '\n' });

  cout << "    Found " << sep_iters.size() << " separators" << endl;

  // Cretate the fault injectors to use
  auto finjectors = make_finjectors<std::string::const_iterator,
                                    std::ostreambuf_iterator<char>>();

  cout << "[+] Loaded " << finjectors.size() << " fault injectors" << endl;

  cout << "[+] Generating faulty files in " << dst_path << endl;

  size_t variant = 0;
  for (auto finjector : finjectors) {
    ostringstream fn;
    fn << src_path.stem().generic_string()
       << "_"
       << std::to_string(variant)
       << src_path.extension().generic_string();

    fs::ofstream ofs(fs::path(dst_path / fn.str()));
    auto osbuf_it = std::ostreambuf_iterator<char>(ofs);

    apply_finjector(str.cbegin(), str.cend(),
                    osbuf_it,
                    sep_iters, finjector);

    ++variant;
  }

  cout << "[+] Generation completed." << endl;

  return 0;
}
