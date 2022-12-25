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

#include "InsertByIter.h"

TEST_P(InsertByIter, Basic) {
    const auto &param = GetParam();
    auto &c = container;

    InsertByIter::Container newValues;
    newValues.reserve(param.values.size());

    std::transform(
        param.values.begin(), param.values.end(),
        inserter(newValues, newValues.end()),
        [](const int value) {
            return vs_test::TestValue(value);
        });

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    c.insert(newValues.begin(), newValues.end());

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}


std::vector<vs_test::InsertByIterParam>
vs_test::InsertByIterParam::caseUnordered = {
    {
        "basic", { -1, 2, 5, 8 }, false,
        /* = */ 10, false, { 8, 0, 2, 5, 2, 6, -1, 2, 5, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"insert(InputIt,InputIt)", "start"}
        }
    }
};


std::vector<vs_test::InsertByIterParam>
vs_test::InsertByIterParam::caseOrdered = {
    {
        "basic", { -1, 2, 5, 8 }, true,
        /* = */ 6, false, { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"insert(InputIt,InputIt)", "start"},
            {"insert(InputIt,InputIt)", "call:make_unique_ordered()"},

            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:6(9)"}
        }
    }
};
