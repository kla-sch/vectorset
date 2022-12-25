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

SRCS = \
	vectorset_test.cpp \
	TestUtils.cpp \
	Constructor.cpp \
	ModeSwitch.cpp \
	ConstructorCopyMove.cpp \
	Insert.cpp \
	InsertPos.cpp \
	InsertByIter.cpp \
	InsertByInitList.cpp \
	Emplace.cpp \
	EmplaceHint.cpp \
	EraseAtPos.cpp \
	EraseRange.cpp \
	EraseByKey.cpp \
	VectorManipulation.cpp \
	MiscManipulationSwap.cpp \
	Count.cpp \
	Find.cpp \
	Contains.cpp \
	RangeBound.cpp \
	Comparators.cpp \
	Allocators.cpp \
	CompBoolOp.cpp \
	CompThreeWayOp.cpp \
	Debug.cpp

OBJS_11 = $(SRCS:%.cpp=objs_c++11/%.o)
OBJS_14 = $(SRCS:%.cpp=objs_c++14/%.o)
OBJS_17 = $(SRCS:%.cpp=objs_c++17/%.o)
OBJS_20 = $(SRCS:%.cpp=objs_c++20/%.o)

OBJS = $(OBJS_11) $(OBJS_14) $(OBJS_17) $(OBJS_20)

VECTORSET_H = ../vectorset.h

EXES = \
	testVectorTest_c++11 \
	testVectorTest_c++14 \
	testVectorTest_c++17 \
	testVectorTest_c++20

CCFLAGS= -Wall -O3 -MMD \
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

testVectorTest_c++11: objs_c++11 $(GOOGLETESTBUILD) $(OBJS_11)
	@echo "link for c++11: $@ "
	@$(CPP) -std=c++11 -o $@  $(OBJS_11) $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++14: objs_c++14 $(GOOGLETESTBUILD) $(OBJS_14)
	@echo "link for c++14: $@ "
	@$(CPP) -std=c++14 -o $@  $(OBJS_14) $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++17: objs_c++17 $(GOOGLETESTBUILD) $(OBJS_17)
	@echo "link for c++17: $@ "
	@$(CPP) -std=c++17 -o $@  $(OBJS_17) $(CCFLAGS) $(LDFLAGS)

testVectorTest_c++20: objs_c++20 $(GOOGLETESTBUILD) $(OBJS_20)
	@echo "link for c++20: $@ "
	@$(CPP) -std=c++20 -o $@  $(OBJS_20) $(CCFLAGS) $(LDFLAGS)

objs_c++11 objs_c++14 objs_c++17 objs_c++20:
	@echo "Create object dir: $@"
	@mkdir $@

$(GOOGLETESTBUILD):
	mkdir $(GOOGLETESTBUILD)
	( cd $(GOOGLETESTBUILD); cmake ..)
	make -C $(GOOGLETESTBUILD)

clean:
	@echo remove object files and executables
	@rm -f $(EXES) $(OBJS)

extra_clean: clean
	@echo extra clean
	@rm -rf objs_c++11 objs_c++14 objs_c++17 objs_c++20

objs_c++11/%.o: %.cpp
	@echo "compile for c++11: $<"
	@$(CPP) -std=c++11 $(CCFLAGS) -c $< -o $@

objs_c++14/%.o: %.cpp
	@echo "compile for c++14: $<"
	@$(CPP) -std=c++14 $(CCFLAGS) -c $< -o $@

objs_c++17/%.o: %.cpp
	@echo "compile for c++17: $<"
	@$(CPP) -std=c++17 $(CCFLAGS) -c $< -o $@

objs_c++20/%.o: %.cpp
	@echo "compile for c++20: $<"
	@$(CPP) -std=c++20 $(CCFLAGS) -c $< -o $@

-include objs_c++11/*.d
-include objs_c++14/*.d
-include objs_c++17/*.d
-include objs_c++20/*.d
