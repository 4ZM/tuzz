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

#include "tuzz/finjectors/insert_finjector.hpp"

#include <algorithm>
#include <cctype>

using namespace tuzz;

insert_finjector::insert_finjector(const std::string& insert, size_t n, tuzz::position::type position)
  : insert_finjector(insert, n, position, false) { }

insert_finjector::insert_finjector(const std::string& insert, size_t n, tuzz::position::type position, bool skip_ws)
  : insert_(insert), n_(n), position_(position), skip_ws_(skip_ws) { }

std::string insert_finjector::inject(const std::string& chunk) {
  std::string output;
  auto out_it = std::back_inserter(output);
  auto first = chunk.cbegin();
  auto end = chunk.cend();

  if (skip_ws_) {
    // Find the first and last non ws char
    first = std::find_if(first, end, isgraph);
    end = std::find_if(chunk.crbegin(), chunk.crend(), isgraph).base();

    // Still need to copy all the leading ws to the output
    std::copy(chunk.cbegin(), first, out_it);
  }

  if (position_ & position::begining) {
		for (size_t i = 0; i < n_; ++i)
			std::copy(insert_.cbegin(), insert_.cend(), out_it);
	}

  if (position_ & position::middle) {
    auto mid = first + (end - first) / 2;
    std::copy(first, mid, out_it);
		for (size_t i = 0; i < n_; ++i)
			std::copy(insert_.cbegin(), insert_.cend(), out_it);
    std::copy(mid, end, out_it);
  }
  else
    std::copy(first, end, out_it);

  if (position_ & position::end) {
		for (size_t i = 0; i < n_; ++i)
			std::copy(insert_.cbegin(), insert_.cend(), out_it);
	}

  // In case there was some ws we skipped at the end, add them last
  std::copy(end, chunk.cend(), out_it);

  return output;
}
