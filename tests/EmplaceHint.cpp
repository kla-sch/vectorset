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

#include "EmplaceHint.h"

TEST_P(EmplaceHint, Basic) {
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

    auto result = c.emplace_hint (pos_iter, param.value);
    EXPECT_EQ(*result, param.value);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseUnorderedPosBegin = {
    {
        "basic", 3, 0, false,
        /* = */ 7, true, { 3, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:0(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:0(7)"}
        }
    }, {
        "duplicate", 2, 0, false,
        /* = */ 7, true, { 2, 8, 0, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:0(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:0(7)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseUnorderedPos2 = {
    {
        "basic", 3, 2, false,
        /* = */ 7, true, { 8, 0, 3, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:2(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:2(7)"}
        }
    }, {
        "duplicate", 2, 2, false,
        /* = */ 7, true, { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:2(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:2(7)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseUnorderedPosEnd = {
    {
        "basic", 3, -1, false,
        /* = */ 7, true, { 8, 0, 2, 5, 2, 6, 3 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:6(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:6(7)"}
        }
    }, {
        "duplicate", 2, -1, false,
        /* = */ 7, true, { 8, 0, 2, 5, 2, 6, 2 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:6(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:6(7)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseUnorderedPosMatch = {
    {
        "duplicate", 2, 2, false,
        /* = */ 7, true, { 8, 0, 2, 2, 5, 2, 6 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "start:2(6)"},
            {"impl_emplace_unordered(const_iterator, Args&&...)",
             "result:2(7)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPosBegin = {
    {
        "basic", 3, 0, true,
        /* = */ 6, true, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:0(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:0"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:0(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:2(6)"},
            {"impl_insert_emplace_unique_ordered", "move-into-position"},

            {"move_into_position", "start:5(6) to 2(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:true"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:2(6)"}
        }
    }, {
        "duplicate", 2, 0, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:0(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:0"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:0(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:1(6)"},
            {"impl_insert_emplace_unique_ordered", "*last>=*iter:exists"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:false"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPos2 = {
    {
        "basic", 1, 2, true,
        /* = */ 6, true, { 0, 1, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:2(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:2"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:2(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:1(6)"},
            {"impl_insert_emplace_unique_ordered", "move-into-position"},

            {"move_into_position", "start:5(6) to 1(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},
            {"move_into_position", "swap:1(6) and 2(6)"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:true"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:1(6)"}
        }
    }, {
        "duplicate", 2, 2, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:2(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:2"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:2(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:1(6)"},
            {"impl_insert_emplace_unique_ordered", "*last>=*iter:exists"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:false"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPosEnd = {
    {
        "basic", 3, -1, true,
        /* = */ 6, true, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:5(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:5"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:2(6)"},
            {"impl_insert_emplace_unique_ordered", "move-into-position"},

            {"move_into_position", "start:5(6) to 2(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:true"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:2(6)"}
        }
    }, {
        "duplicate", 2, -1, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:5(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:5"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "normal-emplace"},

            {"impl_insert_emplace_unique_ordered", "start"},
            {"impl_insert_emplace_unique_ordered", "last:5(6)"},
            {"impl_insert_emplace_unique_ordered", "iter:1(6)"},
            {"impl_insert_emplace_unique_ordered", "*last>=*iter:exists"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "emplaced:false"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "result:1(5)"}
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPosMatchMiddle = {
    {
        "basic", 3, 2, true,
        /* = */ 6, true, { 0, 2, 3, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:2(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:2"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:2(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hin-on-spot:2(6)"},

            {"move_into_position", "start:5(6) to 2(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},
        }
    }, {
        "duplicate", 2, 1, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:1(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:1"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:1(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "exists-on-hint:1(6)"},
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPosMatchBegin = {
    {
        "basic", -1, 0, true,
        /* = */ 6, true, { -1, 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:0(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:0"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:0(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hin-on-spot:0(6)"},

            {"move_into_position", "start:5(6) to 0(6)"},
            {"move_into_position", "swap:4(6) and 5(6)"},
            {"move_into_position", "swap:3(6) and 4(6)"},
            {"move_into_position", "swap:2(6) and 3(6)"},
            {"move_into_position", "swap:1(6) and 2(6)"},
            {"move_into_position", "swap:0(6) and 1(6)"},
        }
    }, {
        "duplicate", 0, 0, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:0(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:0"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:0(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "exists-on-hint:0(6)"},
        }
    }
};


std::vector<vs_test::EmplaceHintParam>
vs_test::EmplaceHintParam::caseOrderedPosMatchEnd = {
    {
        "basic", 10, -1, true,
        /* = */ 6, true, { 0, 2, 5, 6, 8, 10 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:5(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:5"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hin-on-spot:5(6)"},

            {"move_into_position", "start:5(6) to 5(6)"},
        }
    }, {
        "duplicate", 8, 4, true,
        /* = */ 5, true, { 0, 2, 5, 6, 8 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "start:4(5)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintDistance:4"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "valIter:5(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "hintIter:4(6)"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "not-empty"},
            {"impl_emplace_unique_ordered(const_iterator,Args&&...)",
             "exists-on-hint:4(6)"},
        }
    }
};
