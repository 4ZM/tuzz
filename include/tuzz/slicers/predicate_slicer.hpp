#ifndef PREDICATE_SLICER__HPP
#define PREDICATE_SLICER__HPP
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
#include "tuzz/chunk.hpp"

#include <functional>
#include <vector>

namespace tuzz {

struct predicate_slicer final : public tuzz::slicer {
  explicit predicate_slicer(std::function<bool(char)> predicate);
  virtual std::vector<tuzz::chunk> slice(const std::string& input) override;

  // Hack for GCC Bug 53613, remove dtor when fixed
  virtual ~predicate_slicer() noexcept {};

 private:
  std::function<bool(char)> predicate_;
};

}

#endif
