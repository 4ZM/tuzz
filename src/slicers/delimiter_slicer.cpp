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

#include <algorithm>

using namespace tuzz;

template<typename InIt>
delimiter_slicer<InIt>::delimiter_slicer(typename delimiter_slicer<InIt>::delimiter_type delimiter)
  : delimiter_(delimiter) { }

template<typename InIt>
tuzz::chunks<InIt> delimiter_slicer<InIt>::slice(InIt first, InIt end) {
  chunks<InIt> cs;

  auto it = first;
  do {

    auto match = std::find(it, end, delimiter_);

    cs.push_back(std::make_pair(it, match));

    // Advance past the delimiter (unless we are at the end of input)
    if (match != end)
      ++match;

    // Start over from the end + 1 of the last match
    it = match;

  } while (it != end);

  return cs;
}
