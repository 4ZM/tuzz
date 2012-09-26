#ifndef DELIMITER_SLICER__HPP
#define DELIMITER_SLICER__HPP
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

#include "tuzz/slicer.hpp"

#include <iterator>

namespace tuzz {

template <typename InIt>
struct delimiter_slicer final : public tuzz::slicer<InIt> {
  using delimiter_type = typename std::iterator_traits<InIt>::value_type;

  explicit delimiter_slicer(typename delimiter_slicer<InIt>::delimiter_type delimiter);
  virtual tuzz::chunks<InIt> slice(InIt first, InIt end) override;

 private:
  delimiter_type delimiter_;
};

}

#include "../src/slicers/delimiter_slicer.cpp"

#endif
