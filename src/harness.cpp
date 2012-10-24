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
 *
 * Implementation is heavily influenced by SpikeFile : util.c (GPLv2)
 * Copyright (C) 2005 Adam Greene <agreene@idefense.com>
 */

#include <tuzz/harness.hpp>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <vector>
#include <string>
#include <functional>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>
using namespace std;

using namespace tuzz;

void execute_application();
void monitor(int cpid);

bool is_signal_terminal(int sig);
const char** alloc_args(const std::vector<std::string>& argv);

int checked_call(std::function<int(void)> f, const std::string& msg);
void test(int result, const std::string& msg);
void fail(const std::string& msg);

harness_error::harness_error()
  : tuzz_error("Process harness error") { }

harness_error::harness_error(const char* msg)
  : tuzz_error(msg) { }

harness_error::harness_error(const std::string& msg)
  : tuzz_error(msg.c_str()) { }


harness::harness(const std::string& program,
                       const std::vector<std::string>& args,
                       const std::vector<std::string>& env)
  : program_(program), args_(args), env_(env), inherit_env_(false) {
}

harness::harness(const std::string& program,
                       const std::vector<std::string>& args)
  : program_(program), args_(args), inherit_env_(true) {
}

void tuzz::harness::start() {

  // Create child process
  pid_t cpid = checked_call([] () { return fork(); }, "Forking");

  if (cpid == 0) {
    // This branch executes in the new process
    execute_application_();
  }
  else {
    // This executes in the parent process
    monitor(cpid);
  }
}

void tuzz::harness::abort() { }

void tuzz::harness::execute_application_()
{
  // Build process arguments
  //
  // Note: This will allocate memory on the heap that is never
  // explicitly released. It will however be cleaned up by the OS when
  // the process is terminated.
  const char** cargs = alloc_args(args_);
  const char** cenv = (const char**)environ; // Use parent process env as default

  // Don't use parent env? Then set explicitly
  if (!inherit_env_)
    cenv = alloc_args(env_);

  // Wait for the parent process to attach
  checked_call([] { return ptrace(PTRACE_TRACEME, 0, NULL, NULL); }, "Waiting for trace");

  // Launch the application in this process
  execve(program_.c_str(), (char* const*)cargs, (char* const*)cenv);

  // execve never returns on success. Signal failure to start using the
  // exit before initial trap behaviour in the monitor.
  exit(1);
}

void monitor(int cpid) {
  int status;
  bool got_trap_ = false;

  for(;;) { // Process monitoring loop

    // Wait for child to teminate or get some signal.
    //
    // Note: there is no race with the TRACEME call since it doesn't
    // matter if that call occurs before this call, in which case it
    // is not processed, or if it occurs after, in which case it is
    // processed but ignored.
    checked_call([&] { return waitpid(cpid, &status, 0); },
                 "Waiting for child process");

    // TODO: Refactor code below to process_event function()

    if (WIFEXITED(status)) {
      if (!got_trap_)
        throw harness_error("Failed to start program.");

      // Child terminated normally

      // int ret = WEXITSTATUS(status); // arg to exit() or mains return
      // TODO ==> TO LOG : printf("Child terminated normally with status: %d\n", ret);

      break; // Exit proc mon loop
    }
    else if (WIFSIGNALED(status)) {
      if (!got_trap_)
        throw harness_error("Failed to start program - child was signaled. Wierd.");

      // Child terminated by a signal

      // int sig = WTERMSIG(status);
      // TODO ==> TO LOG : printf("Child terminated by signal: %d\n", sig);

      break; // Exit proc mon loop
    }
    else if (WIFSTOPPED(status)) {
      // Child received signal

      int sig = WSTOPSIG(status);
      if (sig == SIGTRAP) {
        // This is the signal sent when we attach with the debugger
        // and the process resumes.
        got_trap_ = true;
        checked_call([=] { return ptrace(PTRACE_CONT, cpid, NULL, 0); },
                     "Continuing after trap signal");
      }
      else {
        // TODO ==> TO LOG : printf("Child received signal %d: ", sig);
        if (is_signal_terminal(sig)) {
          // TODO ==> TO LOG : printf("Terminating child\n", sig);

          // Kill child and abandon it
          checked_call([=] { return ptrace(PTRACE_KILL, cpid, NULL, NULL); },
                       "Killing child");

          checked_call([=] { return ptrace(PTRACE_DETACH, cpid, NULL, NULL); },
                       "Detaching from child");

          break; // Exit proc mon loop
        }

        // TODO ==> TO LOG : printf("Non-terminal, passing on to child\n", sig);

        // Some other signal, pass on to child
        checked_call([=] { return ptrace(PTRACE_CONT, cpid, NULL, sig); },
                     "Passing on sig to child");
      }
    }
  }

  // TODO Logging process exit
}

const char** alloc_args(const std::vector<std::string>& args) {
  if (args.empty()) {
    static const char* nullargs[] = { NULL };
    return nullargs;
  }

  const char** char_args = new const char*[args.size() + 1];

  // Point into the strings
  for (size_t i = 0; i < args.size(); ++i)
    char_args[i] = (char*) args[i].c_str();

  // NULL terminate the array
  char_args[args.size()] = NULL;

  return char_args;
}

bool is_signal_terminal(int sig) {
  switch(sig) {
  case SIGILL  :
  case SIGABRT :
  case SIGFPE  :
  case SIGKILL :
  case SIGSEGV :
  case SIGPIPE :
  case SIGBUS  :
  case SIGSYS  :
  case SIGXCPU :
  case SIGXFSZ :
    return true;
  }
  return false;
}

int checked_call(std::function<int(void)> f, const std::string& msg) {
  int res = f();
  test(res, msg);
  return res;
}

void test(int result, const std::string& msg) {
  if (result < 0) fail(msg);
}

void fail(const std::string& msg) {

  if (errno) {
    // ==> Do logging
    throw harness_error(msg + " : " + strerror(errno));
  }
  else {
    // ==> Do logging
    throw harness_error(msg);
  }
}
