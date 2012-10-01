#ifndef MAGIC_NUMBERS__HPP
#define MAGIC_NUMBERS__HPP
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

#include <vector>

namespace tuzz {

struct magic_numbers {
  explicit magic_numbers(unsigned int distance);

  std::vector<unsigned int> field_lengths() const;
  std::vector<unsigned int> unsigned_numbers() const;
  
private:
  unsigned int distance_;
};


struct magic_number_error : public tuzz_error {
  magic_number_error();
  magic_number_error(const char* msg);
};

}

#endif
