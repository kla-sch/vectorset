#
# Copyright 2022 kla-sch@gmx.de
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
# Targets:
#     all   - compile benchmark program (default)
#     clean - remove object files and benchmark program
#

OBJS=bench_vectorset.o

GOOGLE_BENCH=google_benchmark
GOOGLE_BENCH_BUILD=$(GOOGLE_BENCH)/build

CPP = c++
CCFLAGS = -std=c++17 -O3 -I $(GOOGLE_BENCH)/include
LDFLAGS = -L $(GOOGLE_BENCH_BUILD)/src -lbenchmark -lpthread

.PHONY: all clean

all: runBench

clean:
	rm -f runBench $(OBJS)

runBench: $(OBJS) $(GOOGLE_BENCH_BUILD)
	$(CPP) -o $@ $(OBJS) $(CCFLAGS) $(LDFLAGS)

$(GOOGLE_BENCH_BUILD):
	( \
		cd $(GOOGLE_BENCH); \
		cmake -E make_directory "build"; \
		cmake -E chdir "build" \
			cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on \
				-DCMAKE_BUILD_TYPE=Release ../; \
		cmake --build "build" --config Release; \
	)

%.o:%.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@

$(OBJS): StringGenerator.h
