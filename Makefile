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

.PHONY: clean all test run_tests

all: tuzz tests

TEST_BINS =                               \
  test/test_numbered_string               \
  test/test_cmdline_options               \
  test/test_prng                          \
  test/test_input_source                  \
  test/test_output_target                 \
  test/test_transform_finjector           \
  test/test_repeat_finjector              \
  test/test_insert_finjector              \
  test/test_predicate_slicer              \
  test/test_delimiter_slicer              \
  test/test_any_of_slicer                 \
  test/test_all_slicer

tests: ${TEST_BINS}

run_tests: tests
	$(foreach var,$(TEST_BINS),$(var);)

clean:
	rm -f tuzz *~ ${TEST_BINS} ${TUZZ_OBJS}


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

# Testcases - need a bit of special care since
# dependencies are hard to figure out automatically
test/test_numbered_string: test/src/test_numbered_string.cpp numbered_string.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_cmdline_options: test/src/test_cmdline_options.cpp cmdline_options.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_prng: test/src/test_prng.cpp prng.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_input_source: test/src/test_input_source.cpp input_source.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_transform_finjector: test/src/finjectors/test_transform_finjector.cpp transform_finjector.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_repeat_finjector: test/src/finjectors/test_repeat_finjector.cpp repeat_finjector.o position.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_insert_finjector: test/src/finjectors/test_insert_finjector.cpp insert_finjector.o position.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_delimiter_slicer: test/src/slicers/test_delimiter_slicer.cpp delimiter_slicer.o chunk.o predicate_slicer.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_any_of_slicer: test/src/slicers/test_any_of_slicer.cpp any_of_slicer.o chunk.o predicate_slicer.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_predicate_slicer: test/src/slicers/test_predicate_slicer.cpp predicate_slicer.o chunk.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_all_slicer: test/src/slicers/test_all_slicer.cpp all_slicer.o chunk.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

test/test_output_target: test/src/test_output_target.cpp output_target.o numbered_string.o
	${GCC} -o $@ $^ ${CFLAGS_TEST} ${LDFLAGS_TEST}

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
