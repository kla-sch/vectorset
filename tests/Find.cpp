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

#include "Find.h"

TEST_P(Find, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    auto &c = container;

    auto iter = c.find(vs_test::TestValue(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceKey));
    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, ConstKey) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto iter = c.find(vs_test::TestValue(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceConstKey));
    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    auto &c = container;

    auto iter = c.find(vs_test::MiscInt(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceMisc));
    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, ConstMisc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto iter = c.find(vs_test::MiscInt(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceConstMisc));
    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}


// Test parameters:
std::vector<vs_test::FindParam>
vs_test::FindParam::caseUnordered = {
    {
        "find_0",  0,  false, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:1(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:1(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:1(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:1(6)"}
        }
    },
    {
        "find_2",  2,  false, /*=*/  2,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:2(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:2(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:2(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:2(6)"}
        }
    },
    {
        "find_3",  3,  false, /*=*/ -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:6(6)"}
        }
    },
    {
        "find_6",  6,  false, /*=*/  5,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:5(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:5(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:5(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:5(6)"}
        }
    },
    {
        "find_8",  8,  false, /*=*/  0,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:0(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:0(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:0(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:0(6)"}
        }
    },
    {
        "find_10", 10, false, /*=*/ -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)", "start"},
            {"impl_find_unordered(const_reference)", "start"},
            {"impl_find_unordered(const_reference)", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)", "start"},
            {"impl_find_unordered(const K&)", "start"},
            {"impl_find_unordered(const K&)", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:6(6)"}
        }
    }
};


std::vector<vs_test::FindParam>
vs_test::FindParam::caseOrdered = {
    {
        "find_0",  0,  true, /*=*/  0,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:0(5)"}
        }
    },
    {
        "find_2",  2,  true, /*=*/  1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:1(5)"}
        }
    },
    {
        "find_3",  3,  true, /*=*/ -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:2(5)"}
        }
    },
    {
        "find_6",  6,  true, /*=*/  3,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:3(5)"}
        }
    },
    {
        "find_8",  8,  true, /*=*/  4,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:4(5)"}
        }
    },
    {
        "find_10", 10, true, /*=*/ -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)", "start"},
            {"impl_find_unique_ordered(const_reference)", "start"},
            {"impl_find_unique_ordered(const_reference)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "start"},
            {"impl_find_unique_ordered(const K&)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:5(5)"}
        }
    }
};
