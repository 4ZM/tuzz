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
#include "tuzz/slicers/any_of_slicer.hpp"
#include "tuzz/chunk.hpp"

#include <algorithm>
#include <utility>

using namespace tuzz;

any_of_slicer::any_of_slicer(const std::string& delimiters)
  : delimiters_(delimiters) { }

any_of_slicer::any_of_slicer(std::string&& delimiters)
  : delimiters_(delimiters) { }

std::vector<tuzz::chunk> any_of_slicer::slice(const std::string& input) {
  return predicate_slicer([=] (char c) { return delimiters_.find_first_of(c) != std::string::npos; })
    .slice(input);
}
