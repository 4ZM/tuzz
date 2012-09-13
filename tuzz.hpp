#ifndef TUZZ__HPP
#define TUZZ__HPP
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

#include <functional>
#include <algorithm>

namespace tuzz {

  template <typename InIt, typename OutIt>
  using finjector_t = std::function<OutIt(InIt, InIt, OutIt)>;

  namespace position {
    enum type {
      begining = 1<<0,
      middle   = 1<<1,
      end      = 1<<2,
    };

    type operator|(type t1, type t2) {
      return static_cast<type>(static_cast<int>(t1) | static_cast<int>(t2));
    }
  }
  using position_t = position::type;

  namespace impl {
    template <typename InIt, typename OutIt>
    OutIt rep_n_(InIt first, InIt last, OutIt out, size_t n, position::type pos, std::function<char(InIt)> c) {
      if (pos & position::begining)
        std::fill_n(out, n, c(first));

      if (pos & position::middle) {
        auto mid = first + (last - first) / 2;
        std::copy(first, mid, out);
        std::fill_n(out, n, c(mid));
        std::copy(mid, last, out);
      }
      else
        std::copy(first, last, out);

      if (pos & position::end)
        std::fill_n(out, n, c(last-1));

      return out;
    }
  }

  template <typename InIt, typename OutIt>
  OutIt rep_n(InIt first, InIt last, OutIt out, size_t n, position::type pos) {
    if (first == last) return out;
    return impl::rep_n_<InIt, OutIt>(first, last, out, n, pos, [] (InIt i) { return *i; });
  }

  template <typename InIt, typename OutIt>
  OutIt rep_n(InIt first, InIt last, OutIt out, size_t n, position::type pos, char c, bool between_empty) {
    if (first == last && !between_empty) return out;

    std::function<char(InIt)> f = [=] (InIt i) { return c; };
    return impl::rep_n_(first, last, out, n, pos, f);
  }


}

#endif
