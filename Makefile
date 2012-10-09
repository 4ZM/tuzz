# Copyright (C) 2012 Anders Sundman <anders@4zm.org>
#
# This file is part of tuzz - the text fuzzer.
#
# tuzz is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# tuzz is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with tuzz.  If not, see <http://www.gnu.org/licenses/>.

GCC        = gcc
CFLAGS     = -g -Wall -std=c++11 -I include
LDFLAGS    = -lstdc++                 \
             -lboost_filesystem       \
             -lboost_program_options  \
             -lboost_regex

CFLAGS_TEST  = ${CFLAGS} -I test/include
LDFLAGS_TEST = ${LDFLAGS}

TUZZ_SRCS  =                       \
  numbered_string.cpp              \
  input_source.cpp                 \
  output_target.cpp                \
  cmdline_options.cpp              \
  prng.cpp                         \
  chunk.cpp                        \
  slicer.cpp                       \
  magic_numbers.cpp                \
  position.cpp                     \
  slicers/all_slicer.cpp           \
  slicers/predicate_slicer.cpp     \
  slicers/delimiter_slicer.cpp     \
  slicers/any_of_slicer.cpp        \
  finjector.cpp                    \
  finjectors/transform_finjector.cpp  \
  finjectors/insert_finjector.cpp     \
  finjectors/repeat_finjector.cpp

TUZZ_OBJS1 = $(notdir $(TUZZ_SRCS))
TUZZ_OBJS = $(TUZZ_OBJS1:.cpp=.o)

TEST_SRCS =                       \
  test_runner.cpp                 \
  test_cmdline_options.cpp        \
  test_input_source.cpp           \
  test_numbered_string.cpp        \
  test_output_target.cpp          \
  test_prng.cpp                   \
  finjectors/test_insert_finjector.cpp      \
  finjectors/test_repeat_finjector.cpp      \
  finjectors/test_transform_finjector.cpp   \
  slicers/test_all_slicer.cpp               \
  slicers/test_any_of_slicer.cpp            \
  slicers/test_delimiter_slicer.cpp         \
  slicers/test_predicate_slicer.cpp

TEST_OBJS1 = $(notdir $(TEST_SRCS))
TEST_OBJS = $(TEST_OBJS1:.cpp=.o)

.PHONY: clean all test run_tests

all: tuzz test/test_runner targets harness

TARGET_BINS =                              \
  test/targets/local_overflow              \
  test/targets/remote_overflow

targets: ${TARGET_BINS}

test: test/test_runner
	./test/test_runner

clean:
	rm -f tuzz *~ ${TUZZ_OBJS} ${TEST_OBJS} ${TARGET_BINS} test/test_runner

# Build the tuzz binary
tuzz: src/tuzz.cpp ${TUZZ_OBJS}
	${GCC} -o $@ $^ ${CFLAGS} ${LDFLAGS}

# Build object files from .cpp files in src dir
%.o : src/%.cpp Makefile
	${GCC} ${CFLAGS} -c $<

%.o : src/slicers/%.cpp Makefile
	${GCC} ${CFLAGS} -c $<

%.o : src/finjectors/%.cpp Makefile
	${GCC} ${CFLAGS} -c $<

%.o : test/src/%.cpp Makefile
	${GCC} ${CFLAGS_TEST} -c $<

%.o : test/src/finjectors/%.cpp Makefile
	${GCC} ${CFLAGS_TEST} -c $<

%.o : test/src/slicers/%.cpp Makefile
	${GCC} ${CFLAGS_TEST} -c $<

targets/local_overflow: test/targets/local_overflow.cpp
	${GCC} -o $@ $^ ${CFLAGS} ${LDFLAGS}

targets/remote_overflow: test/targets/remote_overflow.cpp
	${GCC} -o $@ $^ ${CFLAGS} ${LDFLAGS}

# Testcases - need a bit of special care since
# dependencies are hard to figure out automatically
test/test_runner: ${TEST_OBJS} ${TUZZ_OBJS}
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

harness: src/harness.cpp
	gcc -o $@ $^ --std=c++11 -lstdc++

# makedepend section - set up include dependencies
DEPFILE		= .depends
DEPTOKEN	= '\# MAKEDEPENDS'
DEPFLAGS	= -Y -f $(DEPFILE) -s $(DEPTOKEN)

depend:
	rm -f $(DEPFILE)
	make $(DEPFILE)

$(DEPFILE):
	@echo $(DEPTOKEN) > $(DEPFILE)
	makedepend $(DEPFLAGS) -- $(CFLAGS) -- src/*.cpp 2> /dev/null
	makedepend -a -o' ' $(DEPFLAGS) -- $(CFLAGS) -- test/src/*.cpp 2> /dev/null

sinclude $(DEPFILE)
