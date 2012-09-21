#ifndef INPUT_SOURCE__HPP
#define INPUT_SOURCE__HPP
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

struct input_source {
  input_source();
  explicit input_source(const std::string & input_file);

	std::shared_ptr<std::istream> get_stream() const;

private:
  std::shared_ptr<std::istream> source_stream_;
};

struct input_source_error : public tuzz_error {
  input_source_error();
  input_source_error(const char* msg);
};

}

#endif
