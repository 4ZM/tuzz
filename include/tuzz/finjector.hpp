#ifndef FINJECTOR__HPP
#define FINJECTOR__HPP
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

#include <string>
#include <iterator>

namespace tuzz {

template <typename InIt = std::string::const_iterator, typename OutIt = std::back_insert_iterator<std::string>>
struct finjector_base {
  using input_iterator = InIt;
  using output_iterator = OutIt;

  virtual OutIt inject(InIt first, InIt end, OutIt out) = 0;
  virtual ~finjector_base() = default;
};

struct finjector_error : public tuzz_error {
  finjector_error();
  finjector_error(const char* msg);
};

// Convenience definition
using finjector = finjector_base<>;
}

#endif
