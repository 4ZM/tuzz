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

#include "tuzz/chunk.hpp"

#include <string>

using namespace tuzz;

chunk::chunk(std::string::const_iterator begin, std::string::const_iterator end, tuzz::chunk_type type)
  : begin_(begin), end_(end), type_(type) {}

chunk::chunk(std::string::const_iterator begin, std::string::const_iterator end)
  : chunk(begin, end, chunk_type::unclassified) {}

std::string::const_iterator chunk::cbegin() const { return begin_; }

std::string::const_iterator chunk::cend() const { return end_; }

tuzz::chunk_type chunk::type() const { return type_; }

std::string chunk::str() const { return std::string(begin_, end_); }
