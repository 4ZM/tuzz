#ifndef NUMBERED_STRING__HPP
#define NUMBERED_STRING__HPP
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

namespace tuzz {

/**
 * Create strings with embedded numbers according to a pattern
 * specification.
 *
 * The pattern is: prefix%dnsuffix and produces prefix0032suffix if d
 * is 4 or omitted (default) and str(32) is requested.
 */
struct numbered_string {

  /**
   * The pattern is: prefix%dnsuffix and produces prefix0032suffix if d
   * is 4 or omitted (default) and str(32) is requested.
	 *
	 * Will throw numbered_string_format_error if the number place
	 * holder is missing, malformated or if a bad width is specified.
   */
  numbered_string(const std::string& pattern);

  // Get a string with the pattern substituted for the number n.
  std::string str(size_t n) const;

private:
  std::string prefix_;
  std::string suffix_;
  size_t min_digits_ = 4;
  static const size_t max_min_digits_ = 10;
};

struct numbered_string_format_error : public tuzz_error {
  numbered_string_format_error();
};

}

#endif
