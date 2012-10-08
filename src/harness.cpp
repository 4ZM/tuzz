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

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>


void fail(const char* msg) {
  if (errno)
    printf("%s : %s\n", msg, strerror(errno));
  else
    printf("%s\n", msg);

  exit(EXIT_FAILURE);
}
void test(int result, const char* msg) {
  if (result < 0) fail(msg);
}

int is_signal_terminal(int sig) {
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
    return 1;
  }
  return 0;
}

int main(int argc, char* argv[]) {
  static const char* argsep = "--";
  const int copy_env = 1;

  // Find arg separator
  const char* const* cargv = NULL;
  for (int i = 1; i < argc - 1; ++i) {
    if (strcmp(argsep, argv[i]) == 0) {
      cargv = &argv[i + 1]; // Point to arg after sep
      break;
    }
  }

  // Check args
  if (cargv == NULL) {
    printf("Usage: harness -opt1 -opt2 -- target --target-arg\n");
    fail("Argument parse error");
  }

  // Create child process
  pid_t cpid = fork();
  test(cpid, "Forking");

  if (cpid == 0) {
    // In child process

    // Wait for the parent to get ready
    long res = ptrace(PTRACE_TRACEME, 0, NULL, NULL);
    test(res, "Waiting for trace");

    // Execute the child program
    const char* const null_env[] = { NULL };
    execve(cargv[0], (char* const*)cargv, copy_env ? environ : (char* const*)null_env);

    // execve never returns on success
    fail("Executing child program");

    // TODO: Should we use PTRACE_O_TRACEFORK to trace recursive process creation?
  }
  else {
    // In parent process

    int status;
    for(;;) { // Process monitoring loop

      // Wait for child to teminate or get some signal
      int wpid = waitpid(cpid, &status, 0);
      test(wpid, "Waiting for child process");

      if (WIFEXITED(status)) {
        // Child terminated normally
        int ret = WEXITSTATUS(status); // arg to exit() or mains return
        printf("Child terminated normally with status: %d\n", ret);
        break; // Exit proc mon loop
      }
      else if (WIFSIGNALED(status)) {
        // Child terminated by a signal
        int sig = WTERMSIG(status);
        printf("Child terminated by signal: %d\n", sig);
        break; // Exit proc mon loop
      }
      else if (WIFSTOPPED(status)) {
        // Child received signal

        int res = 0;
        int sig = WSTOPSIG (status);
        if (sig == SIGTRAP) {
          printf("Child trapped, continuing...\n");
          res = ptrace(PTRACE_CONT, cpid, NULL, 0);
          test(res, "Continuing after trap signal");
        }
        else {
          printf("Child received signal %d: ", sig);
          if (is_signal_terminal(sig)) {
            printf("Terminating child\n", sig);

            // Do some logging here

            // Kill child and abandon it
            res = ptrace(PTRACE_KILL, cpid, NULL, NULL);
            test(res, "Killing child");

            ptrace(PTRACE_DETACH, cpid, NULL, NULL);
            test(res, "Detaching from child");

            break; // Exit proc mon loop
          }

          printf("Non-terminal, passing on to child\n", sig);

          // Some other signal, pass on to child
          res = ptrace(PTRACE_CONT, cpid, NULL, sig);
          test(res, "Passing on sig to child");
        }
      }
    }
  }

  return 0;
}
