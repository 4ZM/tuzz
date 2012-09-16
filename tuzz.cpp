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
#include <sstream>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <boost/program_options.hpp>

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
  using namespace tuzz;
  namespace fs = boost::filesystem;
  namespace po = boost::program_options;
  using namespace std;

  po::options_description generic("Allowed options");
  generic.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version string")
    ("output,o", po::value<string>()->default_value("-"), "output file pattern (%n)")
    ;

  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("input,i", po::value< vector<string> >(), "input file(s)")
    ;

  po::positional_options_description p;
  p.add("input", -1);

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(hidden);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
            options(cmdline_options).positional(p).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << generic << endl;
    return 1;
  }

  if (vm.count("version")) {
    cout << "tuzz - the text fuzzer - v0.0.1" << endl;
    cout << "Copyright (C) 2012 Anders Sundman <anders@4zm.org>" << endl;
    cout << "This is free software: you are free to change and redistribute it." << endl;
    cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
  }

  if (vm.count("input"))
  {
    vector<string> inputs = vm["input"].as<vector<string>>();
      for(auto i : inputs)
        cout << "Input files are: " << i << endl;
  }

  if (vm.count("output")){
    cout << "output: " << vm["output"].as<string>() << endl;

    if (vm["output"].as<string>() == "-") {
      cout << "-o- is not yet supported" << endl;
      return 1;
    }
  }

  fs::path src_path(vm["input"].as<vector<string>>()[0]);
  fs::path dst_path(vm["output"].as<string>());

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
