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

#include "EraseByKey.h"

TEST_P(EraseByKey, Basic) {
    const auto &param = GetParam();
    auto &c = container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto erase_cnt = c.erase(param.value);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_EQ(erase_cnt, param.expectedErased);
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}


// Test parameters:
std::vector<vs_test::EraseByKeyParam>
vs_test::EraseByKeyParam::caseUnordered = {
    {
        "begin", 8, false,
        /* = */ 1, 5, { 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_erase_unordered(const Key&)", "start"},
            {"impl_erase_unordered(const Key&)", "rmIter:5(6)"},
            {"impl_erase_unordered(const Key&)", "rmSize:1"}
        }
    }, {
        "middle", 5, false,
        /* = */ 1, 5, { 8, 0, 2, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_erase_unordered(const Key&)", "start"},
            {"impl_erase_unordered(const Key&)", "rmIter:5(6)"},
            {"impl_erase_unordered(const Key&)", "rmSize:1"}
        }
    }, {
        "end", 6, false,
        /* = */ 1, 5, { 8, 0, 2, 5, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_erase_unordered(const Key&)", "start"},
            {"impl_erase_unordered(const Key&)", "rmIter:5(6)"},
            {"impl_erase_unordered(const Key&)", "rmSize:1"}
        }
    }, {
        "multiple", 2, false,
        /* = */ 2, 4, { 8, 0, 5, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_erase_unordered(const Key&)", "start"},
            {"impl_erase_unordered(const Key&)", "rmIter:4(6)"},
            {"impl_erase_unordered(const Key&)", "rmSize:2"}
        }
    }, {
        "none", 3, false,
        /* = */ 0, 6, { 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_erase_unordered(const Key&)", "start"},
            {"impl_erase_unordered(const Key&)", "rmIter:6(6)"},
            {"impl_erase_unordered(const Key&)", "rmSize:0"}
        }
    }
};


std::vector<vs_test::EraseByKeyParam>
vs_test::EraseByKeyParam::caseOrdered = {
    {
        "begin", 0, true,
        /* = */ 1, 4, { 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:0(5)"},
            {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},
        }
    }, {
        "middle", 5, true,
        /* = */ 1, 4, { 0, 2, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:2(5)"},
            {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},
        }
    }, {
        "end", 8, true,
        /* = */ 1, 4, { 0, 2, 5, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:4(5)"},
            {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},
        }
    }, {
        "noneBegin", -10, true,
        /* = */ 0, 5, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:0(5)"},
            {"impl_erase_unique_ordered(const Key&)", "not-found:0"},
        }
    }, {
        "noneMiddle", 3, true,
        /* = */ 0, 5, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:2(5)"},
            {"impl_erase_unique_ordered(const Key&)", "not-found:0"},
        }
    }, {
        "noneEnd", 10, true,
        /* = */ 0, 5, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_erase_unique_ordered(const Key&)", "start"},
            {"impl_erase_unique_ordered(const Key&)", "iter:5(5)"},
            {"impl_erase_unique_ordered(const Key&)", "not-found:0"},
        }
    }
};
