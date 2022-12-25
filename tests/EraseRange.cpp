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

#include "EraseRange.h"


TEST_P(EraseRange, Basic) {
    const auto &param = GetParam();
    auto &c = container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto first_iter = (
        param.firstOffset >=0
        ? c.begin() + param.firstOffset
        : c.end());

    auto last_iter = (
        param.lastOffset >=0
        ? c.begin() + param.lastOffset
        : c.end());


    auto erase_iter = c.erase(first_iter, last_iter);

    auto next_iter = c.begin() + param.firstOffset;
    EXPECT_EQ(erase_iter, next_iter);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}


// Test parameters:
std::vector<vs_test::EraseRangeParam>
vs_test::EraseRangeParam::caseUnordered = {
    {
        "begin", 0, 2, false,
        /* = */ 4, { 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator,const_iterator)", "start:0(6) to 2(6)"}
        }
    }, {
        "middle", 2, 4, false,
        /* = */ 4, { 8, 0, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator,const_iterator)", "start:2(6) to 4(6)"}
        }
    }, {
        "end", 4, -1, false,
        /* = */ 4, { 8, 0, 2, 5 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator,const_iterator)", "start:4(6) to 6(6)"}
        }
    }, {
        "all", 0, -1, false,
        /* = */ 0, { },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator,const_iterator)", "start:0(6) to 6(6)"}
        }
    }
};


std::vector<vs_test::EraseRangeParam>
vs_test::EraseRangeParam::caseOrdered = {
    {
        "begin", 0, 2, true,
        /* = */ 3, { 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator,const_iterator)", "start:0(5) to 2(5)"}
        }
    }, {
        "middle", 2, 4, true,
        /* = */ 3, { 0, 2, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator,const_iterator)", "start:2(5) to 4(5)"}
        }
    }, {
        "end", 3, -1, true,
        /* = */ 3, { 0, 2, 5 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator,const_iterator)", "start:3(5) to 5(5)"}
        }
    }, {
        "all", 0, -1, true,
        /* = */ 0, { },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator,const_iterator)", "start:0(5) to 5(5)"}
        }
    }
};
