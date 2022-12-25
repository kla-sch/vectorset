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

#include "Contains.h"


TEST_P(Contains, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto contains = c.contains(vs_test::TestValue(param.value));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceKey));
    EXPECT_EQ(contains, param.expectedContains);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Contains, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto contains = c.contains(vs_test::MiscInt(param.value));

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceMisc));
    EXPECT_EQ(contains, param.expectedContains);
    EXPECT_EQ(c.get_mode(), mode);
}

// Test parameters:
std::vector<vs_test::ContainsParam>
vs_test::ContainsParam::caseUnordered = {
    {
        "contains_0",  0,  false, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:1(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:1(6)"}
        }
    }, {
        "contains_2",  2,  false, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:2(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:2(6)"}
        }
    }, {
        "contains_3",  3,  false, /*=*/  false,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:6(6)"}
        }
    }, {
        "contains_6",  6,  false, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:5(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:5(6)"}
        }
    }, {
        "contains_8",  8,  false, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:0(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:0(6)"}
        }
    }, {
        "contains_10", 10, false, /*=*/  false,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unordered(const_reference)const", "start"},
            {"impl_find_unordered(const_reference)const", "iter:6(6)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "start"},
            {"impl_find_unordered(const K&)const", "iter:6(6)"}
        }
    }
};


std::vector<vs_test::ContainsParam>
vs_test::ContainsParam::caseOrdered = {
    {
        "contains_0",  0,  true, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:0(5)"}
        }
    }, {
        "contains_2",  2,  true, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:1(5)"}
        }
    }, {
        "contains_3",  3,  true, /*=*/  false,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:2(5)"}
        }
    }, {
        "contains_6",  6,  true, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:3(5)"}
        }
    }, {
        "contains_8",  8,  true, /*=*/  true,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:4(5)"}
        }
    }, {
        "contains_10", 10, true, /*=*/  false,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const Key&)const", "start"},
            {"find(const Key&)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "start"},
            {"impl_find_unique_ordered(const_reference)const", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"contains(const K&)const", "start"},
            {"find(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "start"},
            {"impl_find_unique_ordered(const K&)const", "iter:5(5)"}
        }
    }
};
