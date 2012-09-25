#ifndef CMDLINE_OPTIONS__HPP
#define CMDLINE_OPTIONS__HPP
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

#include <string>
#include <ostream>
#include <iostream>
#include <memory>

namespace tuzz {

struct cmdline_options {
  cmdline_options(int argc, const char* argv[], std::ostream & output = std::cout);

  // Explicit, default, dtor required for unique_ptr pimpl.
  ~cmdline_options(); // = default

  bool termination_requested() const;

  void show_usage() const;

  void show_help() const;

  void show_version() const;

  bool is_input_from_stdin() const;
  bool is_output_to_stdout() const;

  std::string get_input_specification() const;
  std::string get_output_specification() const;

private:
  struct impl;
  std::unique_ptr<impl> impl_;
};

}

#endif
