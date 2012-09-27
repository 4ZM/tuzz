#ifndef TRANSFORM_FINJECTOR__HPP
#define TRANSFORM_FINJECTOR__HPP
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

#include "tuzz/finjector.hpp"

#include <functional>

namespace tuzz {

/**
 * Transform the input range, one value at a time, using
 * a specified function.
 */
template <typename InIt, typename OutIt, typename F>
struct transform_finjector_base final : public finjector_base<InIt, OutIt> {

  explicit transform_finjector_base(F f);
  virtual OutIt inject(InIt first, InIt end, OutIt out) override;

  // Hack for GCC Bug 53613, remove dtor when fixed
  virtual ~transform_finjector_base() noexcept {};

private:
  F f_;
};

// Convenience definition
using transform_finjector =
  transform_finjector_base<tuzz::finjector_base<>::input_iterator,
                           tuzz::finjector_base<>::output_iterator,
                           std::function<char(char)>
                           >;
}

#include "../src/finjectors/transform_finjector.cpp"

#endif
