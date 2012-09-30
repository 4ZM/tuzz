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

#include "tuzz/slicers/predicate_slicer.hpp"
#include "tuzz/chunk.hpp"

#include <algorithm>

using namespace tuzz;

predicate_slicer::predicate_slicer(std::function<bool(char)> predicate)
  : predicate_(predicate) { }

std::vector<tuzz::chunk> predicate_slicer::slice(const std::string& input) {
  std::vector<chunk> cs;

  auto it = input.cbegin();
  do {
    // Find the next match (if any)
    auto match = std::find_if(it, input.cend(), predicate_);

    // And store the preceeding chunk
    cs.push_back(chunk(it, match));

    // If there was a matching char, step past it
    if (match != input.cend()) {
      it = ++match;

      // If the match was the very last character, add a last empty chunk
      if (it == input.cend())
        cs.push_back(chunk(it, it));
    }
    else {
      // We are at the end, drop out of the loop
      it = match;
    }
  } while(it != input.cend());

  return cs;
}
