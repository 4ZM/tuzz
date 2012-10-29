#ifndef LOGGER__HPP
#define LOGGER__HPP
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
#include <ostream>
#include <memory>
#include <sstream>

namespace tuzz {

// TODO: Possible logging features. Triage!
//  Target: file, cout, cerr, (syslog, et al.)
//  Verbosity - quiet, normal, (detailed?), debug
//  Severity - Mostly for error logging. Include?
//  Formating - Cout style, printf style?
//  Impl. detail (line no, etc..)

// Collection of free helper functions for interacting with the logger
// singleton instance.
void set_logfile(const std::string& file_name);
void log(const std::string& msg);

struct logger {

  // It's ok to instantiate several loggers, but one global singleton
  // logger is provided for use in the convenience functions.
  // TODO: Evaluate - used DI everywhere?
  logger(std::ostream& target);
  static tuzz::logger& get_logger();
  //  static void set_logger(ilogger&& new_logger);

  tuzz::logger& set_output_target(std::ostream& target);
  tuzz::logger& set_logfile(const std::string& file_name);

  tuzz::logger& log(const std::string& msg) const;
  tuzz::logger& log(const std::string& msg, bool nl) const;
  tuzz::logger& flush() const;

private:
  static logger instance_;
  logger();

  std::shared_ptr<std::ostream> target_;
};

struct log_error : public tuzz_error {
  log_error();
  log_error(const char* msg);
};

// struct end_log {};
// extern const end_log endl;

// template<typename T>
// const tuzz::logger& operator<<(const tuzz::logger &l, T x)
// {
//  if (typeid(T) == typeid(end_log)) {
//    logger::get_logger().flush();
//  }

//  std::stringstream ss;
//  ss << x;
//  return logger::get_logger().log(ss.str(), false);
// }

}

#endif
