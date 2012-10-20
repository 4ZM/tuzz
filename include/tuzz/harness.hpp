#ifndef HARNESS__HPP
#define HARNESS__HPP
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tuzz.	 If not, see <http://www.gnu.org/licenses/>.
 */

#include "tuzz/exception.hpp"

#include <vector>
#include <string>

namespace tuzz {

struct harness {

	harness(const std::string& program,
					const std::vector<std::string>& args);

	harness(const std::string& program,
					const std::vector<std::string>& args,
					const std::vector<std::string>& env);

	// Start program in a new process (shouldn't block)
	void start();

	// Stop the process and the waiting thread
	void abort();

private:
	void execute_application_();

	std::string program_;
	std::vector<std::string> args_;
	std::vector<std::string> env_;

	bool inherit_env_;
};

struct harness_error : public tuzz_error {
	harness_error();
	harness_error(const char* msg);
	harness_error(const std::string& msg);
};

}

#endif
