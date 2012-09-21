#ifndef INPUT_SRC__HPP
#define INPUT_SRC__HPP
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

#include "tuzz/exception.hpp"

#include <string>
#include <istream>
#include <memory>

namespace tuzz {

struct input_src {
  input_src();
  explicit input_src(const std::string & input_file);

  input_src(input_src && other) = default;
  input_src & operator=(input_src && other) = default;
  ~input_src(); // = default for unique_ptr pimpl

  std::istream & get_src() const;

  bool input_from_stdin() const;

private:
  std::unique_ptr<std::istream> src_file_;
};

struct input_src_error : public tuzz_error {
  input_src_error();
  input_src_error(const char* msg);
};

}

#endif
