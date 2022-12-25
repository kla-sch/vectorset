//
// Copyright 2022 kla-sch@gmx.de
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "Debug.h"

#ifdef __VECTOR_SET_DO_TRACE
#undef __VECTOR_SET_DO_TRACE
#endif

#ifdef VECTOR_SET_H
#   error vectorset.h should not be included here
#endif

#define _VECTOR_SET_DEBUG 1
#include "vectorset.h"

struct TestDebugValue {
    int val;

    TestDebugValue() : val(0) {}
    TestDebugValue(int val) noexcept : val(val) {}

    bool operator<(const TestDebugValue &other) const noexcept {
        return (val < other.val);
    }
};


TEST_F(Debug, any) {
    vectorset<TestDebugValue> c = { {0}, {1}, {2}, {3}, {4} };
    const auto &constC = c;

    auto preBegin = std::prev(c.begin());
    auto afterEnd = std::next(c.end());

    ASSERT_DEATH(static_cast<void>(c[5]), "");
    ASSERT_DEATH(static_cast<const void>(constC[5]), "");

    ASSERT_DEATH(c.insert(preBegin, TestDebugValue(1)) , "");
    ASSERT_DEATH(c.insert(afterEnd, TestDebugValue(1)) , "");

    const auto insValue = TestDebugValue(1);

    ASSERT_DEATH(c.insert(preBegin, insValue) , "");
    ASSERT_DEATH(c.insert(afterEnd, insValue) , "");

    ASSERT_DEATH(c.emplace_hint(preBegin, 1) , "");
    ASSERT_DEATH(c.emplace_hint(afterEnd, 1) , "");

    ASSERT_DEATH(c.erase(preBegin) , "");
    ASSERT_DEATH(c.erase(c.begin(), afterEnd) , "");
    ASSERT_DEATH(c.erase(preBegin, c.end()) , "");
}
