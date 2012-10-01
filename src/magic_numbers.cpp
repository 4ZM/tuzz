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

#include "tuzz/magic_numbers.hpp"

#include <vector>

using namespace tuzz;

magic_number_error::magic_number_error()
  : magic_number_error("Magic number error") { }

magic_number_error::magic_number_error(const char* msg)
  : tuzz_error(msg) { }


template <typename T>
void add_neighbours(std::vector<T> & vec,
                    T val,
                    int neg_distance,
                    int pos_distance) {

  for (int i = -neg_distance; i <= pos_distance; ++i)
    vec.push_back(val + i);
}

magic_numbers::magic_numbers(unsigned int distance) : distance_(distance) { }

std::vector<unsigned int> magic_numbers::field_lengths() const {
  static const std::vector<unsigned int> limits = {
    0x40, // Some pow of 2
    0x80,
    0x100,
    0x200,
    0x400,
    0x500,
    0x800,
    0x1000,
    0x10000,
    100,  // Some base 10
    500,
    1000,
  };  // Not including the really long.. it would generate too much data

  // Check for underflow
  if (distance_ > limits[0])
    throw magic_number_error();

  std::vector<unsigned int> numbers;
  numbers.reserve(limits.size() * (distance_ * 2 + 1));

  for (unsigned int i : limits)
    add_neighbours(numbers, i, distance_, distance_);

  return numbers;
}

std::vector<unsigned int> magic_numbers::unsigned_numbers() const {
  static const std::vector<unsigned int> limits = {
    0x40,
    0x80,
    0x100,
    0x200,
    0x400,
    0x500,
    0x800,
    0x1000,
    0x5000,
    0x10000,
    0x50000,
    0x100000,
    0x500000,
    0x1000000,
  };

  // Check for underflow
  if (distance_ > limits[0])
    throw magic_number_error();

  std::vector<unsigned int> numbers;
  numbers.reserve((limits.size() + 1) * (distance_ * 2 + 1));

  for (unsigned int i : limits)
    add_neighbours(numbers, i, distance_, distance_);

  // Don't overflow on the last one
  add_neighbours(numbers, 0xffffffff, distance_, 0);

  return numbers;
}
