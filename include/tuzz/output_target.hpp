#ifndef OUTPUT_TARGET__HPP
#define OUTPUT_TARGET__HPP
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
#include <ostream>
#include <memory>

namespace tuzz {

struct numbered_string;

struct output_target {
  output_target();
  output_target(tuzz::numbered_string&& file_name_generator);

  std::shared_ptr<std::ostream> get_stream(size_t n) const;

private:
  std::unique_ptr<tuzz::numbered_string> ns_;
};

struct output_target_error : public tuzz_error {
  output_target_error();
  output_target_error(const char* msg);
};

}

#endif
