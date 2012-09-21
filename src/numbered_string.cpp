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

#include "tuzz/numbered_string.hpp"

#include <boost/regex.hpp>

#include <iostream>
#include <iomanip>

using namespace tuzz;

numbered_string_format_error::numbered_string_format_error() :
  tuzz_error("Bad format of numbered string pattern") { }

numbered_string::numbered_string(const std::string& pattern) {
  boost::regex ns_pattern(R"(%(\d*)n)");
  boost::smatch match;

  // Search for the number pattern
  if (!boost::regex_search(pattern.cbegin(), pattern.cend(), match, ns_pattern))
    throw numbered_string_format_error();

  // One match for the full %dn and one for the 'd' part.
  if (match.size() != 2)
    throw numbered_string_format_error();

  // There was a minimum length specification
  if (match[1].length() > 0) {
    size_t minw;
    std::istringstream iss(match[1].str());

    if (!(iss >> minw))
      throw numbered_string_format_error();

    // Check that the minimum length is sane
    if (minw < 1 || minw > max_min_digits_)
      throw numbered_string_format_error();

    min_digits_ = minw;
  }

  prefix_ = match.prefix();
  suffix_ = match.suffix();
}

// Create the string corresponding to n
std::string numbered_string::str(size_t n) {
  std::stringstream ss;
  ss << prefix_;
  ss << std::setw(min_digits_) << std::setfill('0') << n;
  ss << suffix_;
  return ss.str();
}
