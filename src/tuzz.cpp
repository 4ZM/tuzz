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

#include "tuzz/tuzz.hpp"
#include "tuzz/cmdline_options.hpp"
#include "tuzz/input_source.hpp"
#include "tuzz/output_target.hpp"
#include "tuzz/numbered_string.hpp"
#include "tuzz/prng.hpp"

#include "tuzz/slicers/all_slicer.hpp"
#include "tuzz/slicers/delimiter_slicer.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

template<typename InIt, typename OutIt>
std::vector<tuzz::finjector_t<InIt, OutIt>> make_finjectors() {
  using namespace tuzz;

  return {
    make_rep_n_finjector<InIt, OutIt>(1, position::begining),
    make_rep_n_finjector<InIt, OutIt>(5, position::end),
    make_rep_n_finjector<InIt, OutIt>
      (3, position::begining | position::middle | position::end),
    make_rep_n_char_finjector<InIt, OutIt>(1, '|', position::end, false),
    make_transform_finjector<InIt, OutIt>(::toupper),
    make_transform_finjector<InIt, OutIt>(::tolower),
  };
}

int main(int argc, const char* argv[]) {
  using namespace tuzz;

  // Parse the command line options
  cmdline_options opt(argc, argv);
  if (opt.termination_requested())
    return 0;

  // Setup input (file or stdin)
  input_source source = opt.is_input_from_stdin()
    ? input_source()
    : input_source(opt.get_input_specification());

  // Setup output (file or stdout)
  output_target target = opt.is_output_to_stdout()
    ? output_target()
    : output_target(numbered_string(opt.get_output_specification()));

  prng random_gen = opt.has_random_seed()
    ? prng(opt.get_random_seed())
    : prng();

  // Read data from the input
  using sbuf_it_t = std::istreambuf_iterator<char>;
  std::shared_ptr<std::istream> in_stream = source.get_stream();
  const std::string str = std::string((sbuf_it_t(*in_stream)), sbuf_it_t());

  // Create the slicers
  std::vector<std::unique_ptr<slicer>> slicers;
  slicers.push_back(std::unique_ptr<slicer>(new all_slicer()));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(',')));

  // Find chunks
  auto sep_iters =
    find_sep<std::initializer_list<char>>(str.cbegin(), str.cend(),
             { ',', ';', ':', '/', '(', ')', '-', '\n' });

  // Cretate the fault injectors to use
  auto finjectors = make_finjectors<std::string::const_iterator,
                                    std::ostreambuf_iterator<char>>();

  size_t variant = random_gen(finjectors.size() - 1);
  std::shared_ptr<std::ostream> stream_ptr = target.get_stream(variant);
  auto osbuf_it = std::ostreambuf_iterator<char>(*stream_ptr);

  apply_finjector(str.cbegin(), str.cend(),
                  osbuf_it,
                  sep_iters, finjectors[variant]);

  return 0;
}
