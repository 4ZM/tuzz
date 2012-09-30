#!/bin/bash

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

if [ $# -lt 1 ]; then
    echo "Too few arguments"
    echo "Usage: ./tuzzrunner.sh tuzz-args -- program program-args"
    exit
fi

# Split the tuzz args and the progra with its args
tuzzargs=()
target=()
argpart="tuzz"
for arg in "$@"
do
    if [ "$argpart" == "tuzz" ]; then
        if [ "$arg" == "--" ]; then
            argpart="target"
        else
            tuzzargs+=($arg)
        fi
    else
        target+=($arg)
    fi
done


if [ ${#target[@]} -lt 1 ]; then
    echo "No target specified"
    echo "Usage: ./tuzzrunner.sh tuzz-args -- program program-args"
    exit
fi

# Run the tuzzer
tmpf=$(mktemp)
while true
do
    ./tuzz ${tuzzargs[@]} > tmpf
    cat tmpf | ${target[@]} &> /dev/null
    if [ $? -gt 127 ]; then
        cp $(tmpf) bomb
        break
    fi
done
