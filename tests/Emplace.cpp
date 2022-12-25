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

#include "Emplace.h"

TEST_P(Emplace, Basic) {
    const auto &param = GetParam();
    auto &c = container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto result = c.emplace(param.value);

    EXPECT_EQ(result.second, param.expectInsert);
    EXPECT_EQ(*result.first, param.value);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

std::vector<vs_test::EmplaceParam>
vs_test::EmplaceParam::caseUnordered = {
    {
        "basic", 3, false,
        /* = */ 7, true, true, { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(Args&&...)", "start"},
            {"impl_emplace_unordered(Args&&...)", "insert:true"},
            {"impl_emplace_unordered(Args&&...)", "iter:6(7)"}
        }
    }, {
        "duplicate", 2, false,
        /* = */ 7, true, true, { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(Args&&...)", "start"},
            {"impl_emplace_unordered(Args&&...)", "insert:true"},
            {"impl_emplace_unordered(Args&&...)", "iter:6(7)"}
        }
    }
};

std::vector<vs_test::EmplaceParam>
vs_test::EmplaceParam::caseOrdered = {
    {
        "basic", 3, true,
        /* = */ 6, true, true, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(Args&&...)", "start"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:2(6)"},
            {"impl_insert_emplace_unique_ordered", "move-into-position"},

            {"move_into_position", "start:5(6) to 2(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},

            {"impl_emplace_unique_ordered(Args&&...)", "insert:true"},
            {"impl_emplace_unique_ordered(Args&&...)", "iter:2(6)"}
        }
    }, {
        "begin", -1, true,
        /* = */ 6, true, true, { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(Args&&...)", "start"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:0(6)"},
            {"impl_insert_emplace_unique_ordered", "move-into-position"},

            {"move_into_position", "start:5(6) to 0(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},
            {"move_into_position", "swap:1(6) and 2(6)"},
            {"move_into_position", "swap:0(6) and 1(6)"},

            {"impl_emplace_unique_ordered(Args&&...)", "insert:true"},
            {"impl_emplace_unique_ordered(Args&&...)", "iter:0(6)"}
        }
    }, {
        "end", 10, true,
        /* = */ 6, true, true, { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(Args&&...)", "start"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:5(6)"},
            {"impl_insert_emplace_unique_ordered", "emplace-at-and"},

            {"impl_emplace_unique_ordered(Args&&...)", "insert:true"},
            {"impl_emplace_unique_ordered(Args&&...)", "iter:5(6)"}
        }
    }, {
        "duplicate", 2, true,
        /* = */ 5, false, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(Args&&...)", "start"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:1(6)"},
            {"impl_insert_emplace_unique_ordered", "*last>=*iter:exists"},

            {"impl_emplace_unique_ordered(Args&&...)", "insert:false"},
            {"impl_emplace_unique_ordered(Args&&...)", "iter:1(5)"}
        }}
};
