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

#include "tuzz/cmdline_options.hpp"
#include "tuzz/input_source.hpp"
#include "tuzz/output_target.hpp"
#include "tuzz/numbered_string.hpp"
#include "tuzz/prng.hpp"
#include "tuzz/magic_numbers.hpp"

#include "tuzz/slicers/all_slicer.hpp"
#include "tuzz/slicers/delimiter_slicer.hpp"

#include "tuzz/finjectors/transform_finjector.hpp"
#include "tuzz/finjectors/repeat_finjector.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>

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

  // Read data from the input
  using sbuf_it_t = std::istreambuf_iterator<char>;
  std::shared_ptr<std::istream> in_stream = source.get_stream();
  const std::string str = std::string((sbuf_it_t(*in_stream)), sbuf_it_t());

  // Create the slicers
  std::vector<std::unique_ptr<slicer>> slicers;
  // Not very usefull -> slicers.push_back(std::unique_ptr<slicer>(new all_slicer()));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(' ')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('\t')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(',')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('.')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(';')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(':')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('/')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('(')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer(')')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('=')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('-')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('+')));
  slicers.push_back(std::unique_ptr<slicer>(new delimiter_slicer('_')));

  magic_numbers num(4);
  std::vector<unsigned int> field_lengths = num.field_lengths();

  std::vector<std::unique_ptr<finjector>> finjectors;
  for (auto n : field_lengths) {
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::begining)));
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::end)));
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::middle)));
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::begining, true)));
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::end, true)));
    finjectors.push_back(std::unique_ptr<finjector>(new repeat_finjector(n, position::middle, true)));
  }
  finjectors.push_back(std::unique_ptr<finjector>(new transform_finjector(::toupper)));
  finjectors.push_back(std::unique_ptr<finjector>(new transform_finjector(::tolower)));

  bool loop_forever = !opt.has_count();
  size_t iteration = opt.has_count() ? opt.get_count() : 1;
  while(loop_forever || iteration > 0) {

    prng random_gen = opt.has_random_seed()
      ? prng(opt.get_random_seed())
      : prng();

    std::vector<chunk> chunks = slicers[random_gen(slicers.size() - 1)]->slice(str);
    
    std::shared_ptr<std::ostream> stream_ptr = target.get_stream(random_gen.get_seed());
    auto osbuf_it = std::ostreambuf_iterator<char>(*stream_ptr);
    
    size_t finjector_i = random_gen(finjectors.size() - 1);
    size_t fuzzed_chunk_i = random_gen(chunks.size() - 1);
    std::string fuzzed_chunk =
      finjectors[finjector_i]->inject(chunks[fuzzed_chunk_i].str());
    
    for (size_t i = 0; i < chunks.size(); ++i) {
      if (i == fuzzed_chunk_i) {
        std::copy(fuzzed_chunk.cbegin(), fuzzed_chunk.cend(), osbuf_it);
      }
      else {
        std::copy(chunks[i].cbegin(), chunks[i].cend(), osbuf_it);
        
      }
    }

    --iteration;
  }
  
  return 0;
}
