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

#include <utility>

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

struct end_log { };
extern const end_log lend;

struct logger {

  // It's ok to instantiate several loggers, but one global singleton
  // logger is provided for use in the convenience functions.
  // TODO: Evaluate - used DI everywhere?
  explicit logger(std::ostream& target);

  // As soon as GCC implements move support for iostream base, add move semantics
  //logger(tuzz::logger&&) noexcept = default;
  //logger& operator=(tuzz::logger&&) noexcept = default;

  ~logger() = default;

  static tuzz::logger& get_logger();

  // Change the output target
  tuzz::logger& reset_output_target();
  tuzz::logger& reset_output_target(std::ostream& target);

  // True if there is a target set
  bool is_logging();

  // Convenience for setting up a file output target
  tuzz::logger& set_logfile(const std::string& file_name);

  // Log a message
  tuzz::logger& log(const std::string& msg) const;

  // Convenience functions for building log entries
  template<typename T>
  tuzz::logger& operator<<(T&& x);
  tuzz::logger& operator<<(const tuzz::end_log&);

  static const end_log endlog;

private:
  static logger instance_;
  logger();

  std::stringstream buf_;
  std::unique_ptr<std::ostream, void(*)(const std::ostream*)> target_;
};


template<typename T>
tuzz::logger& logger::operator<<(T&& x)
{
  if (is_logging())
    {
      // Start accumulating the log text into the buffer
      buf_ << x;
    }
  return *this;
}

inline bool logger::is_logging()
{
  return bool(target_);
}


struct log_error : public tuzz_error {
  log_error();
  log_error(const char* msg);
};

}

#endif
