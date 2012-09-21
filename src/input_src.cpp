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

#include "tuzz/input_src.hpp"

#include <iostream>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;
using namespace tuzz;

input_src_error::input_src_error()
  : tuzz_error("Input source (file or stream) error") { }

input_src_error::input_src_error(const char* msg)
  : tuzz_error(msg) { }

input_src::input_src() { }

input_src::input_src(const std::string & input_file) {

  try {
    fs::path src_path(input_file);

    // Some sanity checks
    if (!fs::exists(src_path))
      throw input_src_error("Input source doesn't exist");
    if (!fs::is_regular_file(src_path))
      throw input_src_error("Input source is not a regular file");

    // Create an input stream
    src_file_ = std::unique_ptr<fs::ifstream>(new fs::ifstream(src_path));

  } catch (const fs::filesystem_error& e) {
    throw input_src_error();
  }
}

input_src::~input_src() = default;

std::istream& input_src::get_src() const {
  if (src_file_)
    return *src_file_;
  return std::cin;
}

bool input_src::input_from_stdin() const {
  return !src_file_;
}
