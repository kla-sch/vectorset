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

#include "Count.h"


TEST_P(Count, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto count = c.count(vs_test::TestValue(param.value));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceKey));
    EXPECT_EQ(count, param.expectedCount);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Count, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto count = c.count(vs_test::MiscInt(param.value));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceMisc));
    EXPECT_EQ(count, param.expectedCount);
    EXPECT_EQ(c.get_mode(), mode);
}

static const auto traceCountKeyUnordered = _Vectorset_Trace {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"count(const Key&)const", "start"},
    {"impl_count_unordered(const_reference)const", "start"}
};

static const auto traceCountMiscUnordered = _Vectorset_Trace {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"count(const K&)const", "start"},
    {"impl_count_unordered(const K&)const", "start"}
};


std::vector<vs_test::CountParam>
vs_test::CountParam::caseUnordered = {
    {
        "count_0",  0,  false, /*=*/  1,
        traceCountKeyUnordered, traceCountMiscUnordered
    }, {
        "count_2",  2,  false, /*=*/  2,
        traceCountKeyUnordered, traceCountMiscUnordered
    }, {
        "count_3",  3,  false, /*=*/  0,
        traceCountKeyUnordered, traceCountMiscUnordered
    }, {
        "count_6",  6,  false, /*=*/  1,
        traceCountKeyUnordered, traceCountMiscUnordered
    }, {
        "count_8",  8,  false, /*=*/  1,
        traceCountKeyUnordered, traceCountMiscUnordered
    }, {
        "count_10", 10, false, /*=*/  0,
        traceCountKeyUnordered, traceCountMiscUnordered
    }
};



std::vector<vs_test::CountParam>
vs_test::CountParam::caseOrdered = {
    {
        "count_0",  0,  true, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:0(5)"}
        }
    }, {
        "count_2",  2,  true, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:1(5)"}
        }
    },
    {
        "count_3",  3,  true, /*=*/  0,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:2(5)"}
        }
    }, {
        "count_6",  6,  true, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:3(5)"}
        }
    }, {
        "count_8",  8,  true, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:4(5)"}
        }
    }, {
        "count_10", 10, true, /*=*/  0,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const Key&)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "start"},
            {"impl_count_unique_ordered(const_reference)const", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"count(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "start"},
            {"impl_count_unique_ordered(const K&)const", "iter:5(5)"}
        }
    }
};
