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

#include "tuzz/slicers/delimiter_slicer.hpp"
#include "tuzz/slicers/predicate_slicer.hpp"
#include "tuzz/chunk.hpp"

#include <algorithm>

using namespace tuzz;

delimiter_slicer::delimiter_slicer(char delimiter)
  : delimiter_(delimiter) { }

std::vector<tuzz::chunk> delimiter_slicer::slice(const std::string& input) {
  return predicate_slicer([=] (char c) { return c == delimiter_; }).slice(input);
}
