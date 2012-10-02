#ifndef INSERT_FINJECTOR__HPP
#define INSERT_FINJECTOR__HPP
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
#include "tuzz/position.hpp"

#include <string>

namespace tuzz {

/**
 */
struct insert_finjector final : public finjector {

  insert_finjector(const std::string& insert, size_t n, tuzz::position::type position);
  insert_finjector(const std::string& insert, size_t n, tuzz::position::type position, bool skip_ws);
  virtual std::string inject(const std::string& chunk) override;

  // Hack for GCC Bug 53613, remove dtor when fixed
  // virtual ~repeat_finjector_base() noexcept {};

private:
	std::string insert_;
  size_t n_;
  position::type position_;
  bool skip_ws_;
};

}

#endif
