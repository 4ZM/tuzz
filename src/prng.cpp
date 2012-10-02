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

#include "tuzz/prng.hpp"

#include <random>

using namespace tuzz;

prng_error::prng_error()
  : tuzz_error("Pseudo random number generator error") { }

prng_error::prng_error(const char* msg)
  : tuzz_error(msg) { }

prng::prng() : seed_(std::mt19937(std::random_device()())()), generator_(seed_) { }

prng::prng(unsigned int seed) : seed_(seed), generator_(seed) { }

unsigned int prng::operator()(unsigned int upper_bound) {
  return std::uniform_int_distribution<unsigned int>(0, upper_bound)(generator_);
}

unsigned int prng::get_seed() const {
  return seed_;
}
