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

#include "tuzz/finjectors/repeat_finjector.hpp"

#include <algorithm>
#include <cctype>

using namespace tuzz;

tuzz::position::type tuzz::position::operator|(type t1, type t2) {
  return static_cast<type>(static_cast<int>(t1) | static_cast<int>(t2));
}


repeat_finjector::repeat_finjector(size_t n, tuzz::position::type position)
  : repeat_finjector(n, position, false) { }

repeat_finjector::repeat_finjector(size_t n, tuzz::position::type position, bool skip_ws)
  : n_(n), position_(position), skip_ws_(skip_ws) { }

std::string repeat_finjector::inject(const std::string& chunk) {
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

  if (first == end)
    return output;

  if (position_ & position::begining)
    std::fill_n(out_it, n_, *first);

  if (position_ & position::middle) {
    auto mid = first + (end - first) / 2;
    std::copy(first, mid, out_it);
    std::fill_n(out_it, n_, *mid);
    std::copy(mid, end, out_it);
  }
  else
    std::copy(first, end, out_it);

  if (position_ & position::end)
    std::fill_n(out_it, n_, *(end-1));

  // In case there was some ws we skipped at the end, add them last
  std::copy(end, chunk.cend(), out_it);

  return output;
}
