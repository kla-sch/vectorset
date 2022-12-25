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

#include "EraseAtPos.h"

TEST_P(EraseAtPos, Basic) {
    const auto &param = GetParam();
    auto &c = container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto pos_iter = (
        param.posOffset >=0
        ? c.begin() + param.posOffset
        : c.end());

    auto erase_iter = c.erase(pos_iter);

    auto next_iter = c.begin() + param.posOffset;
    EXPECT_EQ(erase_iter, next_iter);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}


// Test parameters:
std::vector<vs_test::EraseAtPosParam>
vs_test::EraseAtPosParam::caseUnordered = {
    {
        "begin", 0, false,
        /* = */ 5, { 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator)", "start:0(6)"}
        }
    }, {
        "middle", 3, false,
        /* = */ 5, { 8, 0, 2, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator)", "start:3(6)"}
        }
    }, {
        "end", 5, false,
        /* = */ 5, { 8, 0, 2, 5, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"erase(const_iterator)", "start:5(6)"}
        }
    }
};


std::vector<vs_test::EraseAtPosParam>
vs_test::EraseAtPosParam::caseOrdered = {
    {
        "begin", 0, true,
        /* = */ 4, { 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator)", "start:0(5)"}
        }
    }, {
        "middle", 2, true,
        /* = */ 4, { 0, 2, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator)", "start:2(5)"}
        }
    }, {
        "end", 4, true,
        /* = */ 4, { 0, 2, 5, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"erase(const_iterator)", "start:4(5)"}
        }
    }
};
