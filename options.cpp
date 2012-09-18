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

#include "options.hpp"

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
using namespace std;
using namespace tuzz;

po::options_description create_visible_options_();
po::options_description create_hidden_options_();
po::positional_options_description create_positional_options_();

tuzz_options::tuzz_options(int argc, const char* argv[])
  : visible_options_(create_visible_options_())
{
  create_options_(argc, argv);
  process_options_();
}

void tuzz_options::show_usage() const {
  cout << "Usage: tuzz [options] [input files]" << endl;
}

void tuzz_options::show_help() const {
  show_usage();
  cout << endl << visible_options_ << endl;
}

void tuzz_options::show_version() const {
  cout << "tuzz - the text fuzzer - v0.0.1" << endl;
  cout << "Copyright (C) 2012 Anders Sundman <anders@4zm.org>" << endl;
  cout << "This is free software: you are free to change and redistribute it." << endl;
  cout << "There is NO WARRANTY, to the extent permitted by law." << endl;
}

bool tuzz_options::stdin_as_input() const {
  if (!vm_.count("input"))
    return true;

  auto ivec = vm_["input"].as<vector<string>>();
  if (ivec.size() == 1 &&  ivec[0] == "-")
    return true;

  return false;
}

bool tuzz_options::stdout_as_output() const {
  return vm_.count("output") == 0 ||
    (vm_.count("output") == 1 && vm_["output"].as<string>() == "-");
}

vector<boost::filesystem::path> tuzz_options::get_input_paths() const {
  vector<boost::filesystem::path> v;
  for (auto input : vm_["input"].as<vector<string>>())
    v.push_back(input);
  return v;
}

std::string tuzz_options::get_output_pattern() const {
  return vm_.count("output") ? vm_["output"].as<string>() : "";
}

void tuzz_options::create_options_(int argc, const char* argv[]) {
  auto visible = create_visible_options_();
  auto hidden = create_hidden_options_();
  auto positionals = create_positional_options_();

  po::options_description cmdline_options;
  cmdline_options.add(visible).add(hidden);

  try {
    po::store(po::command_line_parser(argc, argv)
              .options(cmdline_options)
              .positional(positionals)
              .run(), vm_);
  }
  catch (const po::invalid_command_line_syntax&) {
    cout << "Invalid command line syntax" << endl;
    show_usage();
    cout << "Try -h for a list of allowed options" << endl;
    exit(1);
  }
  catch (const po::unknown_option&) {
    cout << "Unknown option" << endl;
    show_usage();
    cout << "Try -h for a list of allowed options" << endl;
    exit(1);
  }

  po::notify(vm_);
}

void tuzz_options::process_options_() {
  if (vm_.count("help")) {
    show_help();
    exit(0);
  }

  if (vm_.count("version")) {
    show_version();
    exit(0);
  }
}

boost::program_options::options_description create_visible_options_() {
  po::options_description visible("Allowed options");
  visible.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version string")
    ("output,o", po::value<string>()->default_value("-"), "output file pattern (%n)")
    ;

  return visible;
}

boost::program_options::options_description create_hidden_options_() {
  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("input,i", po::value<vector<string>>(), "input file(s)")
    ;

  return hidden;
}

boost::program_options::positional_options_description create_positional_options_() {
  po::positional_options_description p;
  p.add("input", -1);
  return p;
}
