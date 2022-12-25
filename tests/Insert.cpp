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

#include "Insert.h"

TEST_P(Insert, Basic) {
    const auto &param = GetParam();
    auto &c = container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    if (param.move) {
        auto value = vs_test::TestValue(param.value);
        auto result = c.insert(std::move(value));

        EXPECT_EQ(result.second, param.expectInsert);
        EXPECT_EQ(*result.first, param.value);
    }
    else {
        const auto value = vs_test::TestValue(param.value);
        auto result = c.insert(value);

        EXPECT_EQ(result.second, param.expectInsert);
        EXPECT_EQ(*result.first, param.value);
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

// Test parameters:
std::vector<vs_test::InsertParam>
vs_test::InsertParam::caseUnordered = {
    {
        "basic", 3, false, false,
        /* = */ 7, true, false, { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(ValType&&)", "start"},
            {"impl_insert_unordered(ValType&&)", "push_back:6(7)"}
        }
    }, {
        "basicMove", 3, false, true,
        /* = */ 7, true, true,  { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(ValType&&)", "start"},
            {"impl_insert_unordered(ValType&&)", "push_back:6(7)"}
        }
    }, {
        "duplicate", 2, false, false,
        /* = */ 7, true, false, { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(ValType&&)", "start"},
            {"impl_insert_unordered(ValType&&)", "push_back:6(7)"}
        }
    }, {
        "duplicateMove", 2, false, true,
        /* = */ 7, true, true,  { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(ValType&&)", "start"},
            {"impl_insert_unordered(ValType&&)", "push_back:6(7)"}
        }
    }
};


std::vector<vs_test::InsertParam>
vs_test::InsertParam::caseOrdered = {
    {
        "basic", 3, true, false,
        /* = */ 6, true, false, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},
        }
    }, {
        "basicMove", 3, true, true,
        /* = */ 6, true, true,  { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},
        }
    }, {
        "begin", -1, true, false,
        /* = */ 6, true, false, { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:0(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:0(6)"},
        }
    }, {
        "beginMove", -1, true, true,
        /* = */ 6, true, true,  { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:0(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:0(6)"},
        }
    }, {
        "end", 10, true, false,
        /* = */ 6, true, false, { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:5(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:5(6)"},
        }
    }, {
        "endMove", 10, true, true,
        /* = */ 6, true, true,  { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:5(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:5(6)"},
        }
    }, {
        "duplicate", 2, true, false,
        /* = */ 5, false, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},
        }
    }, {
        "duplicateMove", 2, true, true,
        /* = */ 5, false, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},
        }
    }
};
