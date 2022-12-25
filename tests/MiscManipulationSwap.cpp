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

#include "MiscManipulationSwap.h"


TEST_P(MiscManipulationSwap, Swap) {
    const auto &param = GetParam();

    auto firstMode =
        (param.firstOrdered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    auto secondMode =
        (param.secondOrdered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    auto &c = container;
    c.set_mode(firstMode);
    auto firstSize = c.size();

    vectorset<vs_test::TestValue> newC;
    std::transform(
        param.secondValues.begin(), param.secondValues.end(),
        std::inserter(newC, newC.end()),
        [] (const int value) {
            return vs_test::TestValue(value);
        });
    newC.clearDebugTrace();

    newC.set_mode(secondMode);
    auto secondSize = newC.size();

    c.swap(newC);

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.trace));
    EXPECT_TRUE(vs_test::checkTrace(newC.getDebugTrace(), param.newTrace));

    EXPECT_EQ(c.size(), secondSize);
    EXPECT_EQ(c.get_mode(), secondMode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedSecondResult);

    EXPECT_EQ(newC.size(), firstSize);
    EXPECT_EQ(newC.get_mode(), firstMode);
    EXPECT_TRUE(hasNoCopiedElem(newC));
    EXPECT_EQ(newC, param.expectedFirstResult);
}


// Test parameters:
std::vector<vs_test::MiscManipulationSwapParam>
vs_test::MiscManipulationSwapParam::caseAny = {
    {
        "unorderedEmptyUnordered",
        false, false, {},
        /* = */ { 8, 0, 2, 5, 2, 6 }, {},
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"}
        }
    }, {
        "unorderedUnordered",
        false, false, { 7, -1, 1, 4, 1, 5 },
        /* = */ { 8, 0, 2, 5, 2, 6 }, { 7, -1, 1, 4, 1, 5 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"}
        }
    }, {
        "orderedEmptyUnordered",
        true, false, {},
        /* = */ { 0, 2, 5, 6, 8 }, {},
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"}
        }
    }, {
        "orderedUnordered",
        true, false, { 7, -1, 1, 4, 1, 5 },
        { 0, 2, 5, 6, 8 }, { 7, -1, 1, 4, 1, 5 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"}
        }},
    MiscManipulationSwapParam{
        "unorderedOrdered",
        false, true, { 7, -1, 1, 4, 1, 5 },
        /* = */ { 8, 0, 2, 5, 2, 6 }, { -1, 1, 4, 5, 7 },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-equal"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"}
        }
    }, {
        "orderedEmptyOrdered",
        true, true, {},
        /* = */ { 0, 2, 5, 6, 8 }, {},
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"set_mode(vectorset_mode)", "mode-ordered"}
        }
    }, {
        "orderedOrdered",
        true, true, { 7, -1, 1, 4, 1, 5 },
        /* = */ { 0, 2, 5, 6, 8 }, { -1, 1, 4, 5, 7},
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},
            {"swap(vectorset&)", "start"}
        },
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"}
        }
    }
};
