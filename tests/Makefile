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
#     all   - compile all test programs (default)
#     c++11 - compile test program for C++11
#     c++14 - compile test program for C++14
#     c++17 - compile test program for C++17
#     c++20 - compile test program for C++20
#     check - compile and start all test programs
#     clean - remove object files and programs

ALL = c++11 c++14 c++17 c++20

GOOGLETESTPATH = googletest
GOOGLETESTBUILD = $(GOOGLETESTPATH)/build

CPP = c++
# CPP = clang++

OBJS = \
	vectorset_test_c++11.o \
	vectorset_test_c++14.o \
	vectorset_test_c++17.o \
	vectorset_test_c++20.o

VECTORSET_H = ../vectorset.h

EXES = \
	testVectorTest_c++11 \
	testVectorTest_c++14 \
	testVectorTest_c++17 \
	testVectorTest_c++20

CCFLAGS= -Wall -O3 \
	-I.. -I$(GOOGLETESTPATH)/googletest/include

LDFLAGS = -L$(GOOGLETESTBUILD)/lib  -lgtest -lpthread

.PHONY: all check c++11 c++14 c++17 c++20 clean

all: $(ALL)

c++11: testVectorTest_c++11
c++14: testVectorTest_c++14
c++17: testVectorTest_c++17
c++20: testVectorTest_c++20

check: $(EXES)
	./testVectorTest_c++11 --gtest_brief=1
	./testVectorTest_c++14 --gtest_brief=1
	./testVectorTest_c++17 --gtest_brief=1
	./testVectorTest_c++20 --gtest_brief=1


testVectorTest_c++11: $(GOOGLETESTBUILD) vectorset_test_c++11.o
	$(CPP) -o $@  vectorset_test_c++11.o -std=c++11 $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++14: $(GOOGLETESTBUILD) vectorset_test_c++14.o
	$(CPP) -o $@  vectorset_test_c++14.o -std=c++14 $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++17: $(GOOGLETESTBUILD) vectorset_test_c++17.o
	$(CPP) -o $@  vectorset_test_c++17.o -std=c++14 $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++20: $(GOOGLETESTBUILD) vectorset_test_c++20.o
	$(CPP) -o $@  vectorset_test_c++20.o -std=c++14 $(CCFLAGS) $(LDFLAGS)


$(GOOGLETESTBUILD):
	mkdir $(GOOGLETESTBUILD)
	( cd $(GOOGLETESTBUILD); cmake ..)
	make -C $(GOOGLETESTBUILD)

vectorset_test_c++11.o: vectorset_test.cpp $(VECTORSET_H)
	$(CPP) -std=c++11 $(CCFLAGS) -c vectorset_test.cpp -o $@

vectorset_test_c++14.o: vectorset_test.cpp $(VECTORSET_H)
	$(CPP) -std=c++14 $(CCFLAGS) -c vectorset_test.cpp -o $@

vectorset_test_c++17.o: vectorset_test.cpp $(VECTORSET_H)
	$(CPP) -std=c++17 $(CCFLAGS) -c vectorset_test.cpp -o $@

vectorset_test_c++20.o: vectorset_test.cpp $(VECTORSET_H)
	$(CPP) -std=c++20 $(CCFLAGS) -c vectorset_test.cpp -o $@

clean:
	rm -f $(EXES) $(OBJS)

%.o:%.cpp
	$(CPP) $(CCFLAGS) -c $< -o $@
