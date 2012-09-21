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

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace fs = boost::filesystem;
namespace po = boost::program_options;
using namespace tuzz;

po::options_description create_visible_options_();
po::options_description create_hidden_options_();
po::positional_options_description create_positional_options_();

struct cmdline_options::impl {
  impl(std::ostream & output)
    : out(output), visible_options(create_visible_options_()) { }

  void process_options_(int argc, const char* argv[]) {
    auto hidden = create_hidden_options_();
    auto positionals = create_positional_options_();

    po::options_description cmdline_options;
    cmdline_options.add(visible_options).add(hidden);

    po::store(po::command_line_parser(argc, argv)
              .options(cmdline_options)
              .positional(positionals)
              .run(), vm);

    po::notify(vm);
  }

  bool termination_requested = true;
  std::ostream & out;
  po::variables_map vm;
  po::options_description visible_options;
};

cmdline_options::cmdline_options(int argc, const char* argv[], std::ostream & output) {
  impl_ = std::unique_ptr<impl>(new impl(output));

  try {
    impl_->process_options_(argc, argv);

    if (impl_->vm.count("help")) {
      show_help();
      return;
    }

    if (impl_->vm.count("version")) {
      show_version();
      return;
    }

    // None of the terminating options were present
    impl_->termination_requested = false;
  }
  catch (const po::invalid_command_line_syntax&) {
    impl_->out << "Invalid command line syntax" << std::endl;
    show_usage();
    impl_->out << "Try -h for a list of allowed options" << std::endl;
  }
  catch (const po::unknown_option&) {
    impl_->out << "Unknown option" << std::endl;
    show_usage();
    impl_->out << "Try -h for a list of allowed options" << std::endl;
  }
}

// Required by the unique_ptr pimpl
cmdline_options::~cmdline_options() = default;

void cmdline_options::show_usage() const {
  impl_->out << "Usage: tuzz [options] [input files]" << std::endl;
}

void cmdline_options::show_help() const {
  show_usage();
  impl_->out << std::endl << impl_->visible_options << std::endl;
}

void cmdline_options::show_version() const {
  impl_->out << "tuzz - the text fuzzer - v0.0.1" << std::endl;
  impl_->out << "Copyright (C) 2012 Anders Sundman <anders@4zm.org>" << std::endl;
  impl_->out << "This is free software: you are free to change and redistribute it." << std::endl;
  impl_->out << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
}

bool cmdline_options::termination_requested() const {
  return impl_->termination_requested;
}

bool cmdline_options::input_from_stdin() const {
  return !impl_->vm.count("input") || impl_->vm["input"].as<std::string>() == "-";
}

bool cmdline_options::output_to_stdout() const {
  return !impl_->vm.count("output") || impl_->vm["output"].as<std::string>() == "-";
}

std::string cmdline_options::get_input_specification() const {
  return input_from_stdin() ? "-" : impl_->vm["input"].as<std::string>();
}

std::string cmdline_options::get_output_specification() const {
  return output_to_stdout() ? "-" : impl_->vm["output"].as<std::string>();
}

boost::program_options::options_description create_visible_options_() {
  po::options_description visible("Allowed options");
  visible.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version string")
    ("output,o", po::value<std::string>()->default_value("-"), "output file pattern (%dn) or -")
    ;

  return visible;
}

boost::program_options::options_description create_hidden_options_() {
  po::options_description hidden("Hidden options");
  hidden.add_options()
    ("input,i", po::value<std::string>()->default_value("-"), "input file or -")
    ;

  return hidden;
}

boost::program_options::positional_options_description create_positional_options_() {
  po::positional_options_description p;
  p.add("input", -1);
  return p;
}
