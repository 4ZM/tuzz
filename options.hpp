#ifndef OPTIONS__HPP
#define OPTIONS__HPP
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

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace tuzz {

struct tuzz_options {
  tuzz_options(int argc, const char* argv[]);

  void show_usage() const;

  void show_help() const;

  void show_version() const;

  bool stdin_as_input() const;

  bool stdout_as_output() const;

  std::vector<boost::filesystem::path> get_input_paths() const;
  std::string get_output_pattern() const;

private:
  void create_options_(int argc, const char* argv[]);
  void process_options_();

  boost::program_options::variables_map vm_;
  boost::program_options::options_description visible_options_;
};

}

#endif
