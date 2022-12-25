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

#include "InsertPos.h"

TEST_P(InsertPos, Basic) {
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

    auto value = vs_test::TestValue(param.value);
    if (param.move) {
        auto result = c.insert(pos_iter, std::move(value));

        EXPECT_EQ(*result, param.value);
    }
    else {
        auto result = c.insert(pos_iter, value);

        EXPECT_EQ(*result, param.value);
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

// Test parameters:
std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseUnorderedPosBegin = {
    {
        "basic", 3, 0, false, false,
        /* = */ 7, false, { 3, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:0(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:0(7)"}
        }
    }, {
        "basicMove", 3, 0, false, true,
        /* = */ 7, true,  { 3, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:0(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:0(7)"}
        }
    }, {
        "duplicate", 2, 0, false, false,
        /* = */ 7, false, { 2, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:0(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:0(7)"}
        }
    }, {
        "duplicateMove", 2, 0, false, true,
        /* = */ 7, true,  { 2, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:0(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:0(7)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseUnorderedPos2 = {
    {
        "basic", 3, 2, false, false,
        /* = */ 7, false, { 8, 0, 3, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }, {
        "basicMove", 3, 2, false, true,
        /* = */ 7, true,  { 8, 0, 3, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }, {
        "duplicate", 2, 2, false, false,
        /* = */ 7, false, { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }, {
        "duplicateMove", 2, 2, false, true,
        /* = */ 7, true,  { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseUnorderedPosEnd = {
    {
        "basic", 3, -1, false, false,
        /* = */ 7, false, { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:6(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:6(7)"}
        }
    }, {
        "basicMove", 3, -1, false, true,
        /* = */ 7, true,  { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:6(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:6(7)"}
        }
    }, {
        "duplicate", 2, -1, false, false,
        /* = */ 7, false, { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:6(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:6(7)"}
        }
    }, {
        "duplicateMove", 2, -1, false, true,
        /* = */ 7, true,  { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:6(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:6(7)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseUnorderedPosMatch = {
    {
        "duplicate", 2, 2, false, false,
        /* = */ 7, false, { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }, {
        "duplicateMove", 2, 2, false, true,
        /* = */ 7, true,  { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "start:2(6)"},
            {"impl_insert_unordered(const_iterator pos, ValType&& value)",
             "result:2(7)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPosBegin = {
    {
        "basic", 3, 0, true, false,
        /* = */ 6, false, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:2(6)"}
        }
    }, {
        "basicMove", 3, 0, true, true,
        /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:2(6)"}
        }
    }, {
        "duplicate", 2, 0, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }, {
        "duplicateMove", 2, 0, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPos2 = {
    {
        "basic", 1, 2, true, false,
        /* = */ 6, false, { 0, 1, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:1(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(6)"}
        }
    }, {
        "basicMove", 1, 2, true, true,
        /* = */ 6, true,  { 0, 1, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:1(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(6)"}
        }
    }, {
        "duplicate", 2, 2, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }, {
        "duplicateMove", 2, 2, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPosEnd = {
    {
        "basic", 3, -1, true, false,
        /* = */ 6, false, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:2(6)"}
        }
    }, {
        "basicMove", 3, -1, true, true,
        /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:true"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:2(6)"}
        }
    }, {
        "duplicate", 2, -1, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }, {
        "duplicateMove", 2, -1, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "call:impl_insert_unique_ordered(ValType&&)"},

            {"impl_insert_unique_ordered(ValType&&)", "start"},
            {"impl_insert_unique_ordered(ValType&&)", "lower_bound:1(5)"},
            {"impl_insert_unique_ordered(ValType&&)", "found-no-insert"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert:false"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPosMatchMiddle = {
    {
        "basic", 3, 2, true, false,
        /* = */ 6, false, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:2(6)"}
        }
    }, {
        "basicMove", 3, 2, true, true,
        /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:2(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:2(6)"}
        }
    }, {
        "duplicate", 2, 1, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:1(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:1"}
        }
    }, {
        "duplicateMove", 2, 1, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:1(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:1"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPosMatchBegin = {
    {
        "basic", -1, 0, true, false,
        /* = */ 6, false, { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:0(6)"}
        }
    }, {
        "basicMove",-1, 0, true, true,
        /* = */ 6, true,  { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:0(6)"}
        }
    }, {
        "duplicate", 0, 0, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:0"}
        }
    }, {
        "duplicateMove", 0, 0, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:0(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:0"}
        }
    }
};


std::vector<vs_test::InsertPosParam>
vs_test::InsertPosParam::caseOrderedPosMatchEnd = {
    {
        "basic", 10, -1, true, false,
        /* = */ 6, false, { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:5(6)"}
        }
    }, {
        "basicMove", 10, -1, true, true,
        /* = */ 6, true,  { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:5(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "insert-at-position:5(6)"}
        }
    }, {
        "duplicate", 8, 4, true, false,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:4(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:4"}
        }
    }, {
        "duplicateMove", 8, 4, true, true,
        /* = */ 5, true,  { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "start:4(5)"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "not-empty"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "fount-at-position-no-insert"},
            {"impl_insert_unique_ordered(const_iterator,ValType&&)",
             "next-to-distance:4"}
        }
    }
};
