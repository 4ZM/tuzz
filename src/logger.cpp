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

#include "tuzz/logger.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <chrono>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;
using namespace tuzz;

log_error::log_error()
  : tuzz_error("Logger error") { }

log_error::log_error(const char* msg)
  : tuzz_error(msg) { }


logger::logger() : target_(nullptr, [] (const std::ostream*) { }) { }
logger::logger(std::ostream& target) : target_(&target, [] (const std::ostream*) { }) { }

tuzz::logger& logger::get_logger() {
  return logger::instance_;
}

tuzz::logger& logger::set_logfile(const std::string& file_name) {
  target_ = nullptr;
  try {
    // Create an output stream, append to file
    fs::path target_path(file_name);
    target_.reset(new fs::ofstream(target_path, std::ios_base::out | std::ios_base::app));
  } catch (const fs::filesystem_error& e) {
    throw log_error();
  }
  return get_logger();
}

tuzz::logger& logger::reset_output_target() {
  target_.reset();
  return *this;
}

tuzz::logger& logger::reset_output_target(std::ostream& target) {
  target_.reset(&target);
  return *this;
}

tuzz::logger& logger::operator<<(const tuzz::end_log&)
  {
    if (is_logging())
    {
      // Process, then clear the buffer
      log(buf_.str());
      buf_.str(std::string());
      target_->flush();
    }
    return *this;
  }

const end_log tuzz::lend;

tuzz::logger& logger::log(const std::string& msg) const {
  // Nop before the log file has been set up
  if (!target_)
    return get_logger();

  using sysclock = std::chrono::system_clock;

  //std::chrono::time_point<std::chrono::system_clock> start, end;
  //auto now = sysclock::now();a
  //  auto tt = std::chrono::system_clock::to_time_t(now);
  //auto t = std::localtime(&tt);
  //(*target_) << now << "|" << msg << std::endl;
  //std::cout << sysclock::now();
  *target_ << "time goes here" << "|" << msg;
  *target_ << std::endl;
  return get_logger();
}

void tuzz::set_logfile(const std::string& file_name) {
  logger::get_logger().set_logfile(file_name);
}

tuzz::logger tuzz::logger::instance_;
