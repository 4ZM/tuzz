#ifndef PRNG__HPP
#define PRNG__HPP
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

#include <random>

namespace tuzz {

struct prng {
  // Create a pseudo random generator that will use a seed from the system.
  prng();

  // Create a pseudo random generator that will use the specified seed.
  explicit prng(unsigned int seed);

  // Get the next random number from the range [0,max]
  unsigned int operator()(unsigned int upper_bound);

private:
  std::mt19937 generator_;
};

struct prng_error : public tuzz_error {
  prng_error();
  prng_error(const char* msg);
};

}

#endif
