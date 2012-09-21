#ifndef EXCEPTION__HPP
#define EXCEPTION__HPP
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

namespace tuzz {

struct tuzz_error {
  explicit tuzz_error(const char* what) : what_(what) { }

  const char* what() const noexcept { return what_; }

  // Default copy, move & assignments
  tuzz_error(const tuzz::tuzz_error& e) = default;
  tuzz::tuzz_error& operator=(const tuzz::tuzz_error& e) = default;
  tuzz_error(tuzz::tuzz_error&& e) = default;
  tuzz::tuzz_error& operator=(tuzz::tuzz_error&& e) = default;

private:
  const char* what_;
};

}

#endif
