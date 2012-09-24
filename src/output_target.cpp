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

#include "tuzz/output_target.hpp"
#include "tuzz/numbered_string.hpp"

#include <iostream>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;
using namespace tuzz;

output_target_error::output_target_error()
  : tuzz_error("Output target (file or stream) error") { }

output_target_error::output_target_error(const char* msg)
  : tuzz_error(msg) { }

output_target::output_target() : ns_(nullptr) { }

output_target::output_target(tuzz::numbered_string&& file_name_generator) {
  ns_ = std::unique_ptr<numbered_string>(new numbered_string(std::forward<numbered_string>(file_name_generator)));
}

std::shared_ptr<std::ostream> output_target::get_stream(size_t n) const {
  // No numberer? Point to stdout and use nop deleter
  if (!ns_) {
    return std::shared_ptr<std::ostream>(&std::cout, [] (std::ostream*) { });
  }

  // Numberer, create/open file for output
  std::string file_name = ns_->str(n);
  try {
    fs::path target_path(file_name);

    // Some sanity checks
    if (fs::exists(target_path))
      throw output_target_error("Output file already exist");

    // Create an output stream
    return std::shared_ptr<fs::ofstream>(new fs::ofstream(target_path));

  } catch (const fs::filesystem_error& e) {
    throw output_target_error();
  }
}
