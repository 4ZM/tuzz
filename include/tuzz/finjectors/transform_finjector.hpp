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
struct transform_finjector final : public finjector {

  explicit transform_finjector(std::function<char(char)> f);
  virtual std::string inject(const std::string& chunk) override;

  // Hack for GCC Bug 53613, remove dtor when fixed
  virtual ~transform_finjector() noexcept {};

private:
  std::function<char(char)> f_;
};

}

#endif
