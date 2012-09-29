#ifndef CHUNK__HPP
#define CHUNK__HPP
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

namespace tuzz {

enum class chunk_type {
  unclassified,
  unknown,
  binary,
};

struct chunk {
  explicit chunk(std::string::const_iterator begin, std::string::const_iterator end);
  explicit chunk(std::string::const_iterator begin, std::string::const_iterator end, tuzz::chunk_type type);

  std::string::const_iterator cbegin() const;
  std::string::const_iterator cend() const;
  tuzz::chunk_type type() const;

  std::string str() const;

private:
  std::string::const_iterator begin_;
  std::string::const_iterator end_;
  tuzz::chunk_type type_;
};

}

#endif
