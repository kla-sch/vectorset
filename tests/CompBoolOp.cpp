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

#include "CompBoolOp.h"


TEST_P(CompBoolOp, any) {
    const auto &param = GetParam();
    const auto &c = container;

    CompBoolOp::Container compVals;
    std::transform(
        param.values.begin(), param.values.end(),
        inserter(compVals, compVals.end()),
        [](const int value) {
            return vs_test::TestValue(value);
        });

    EXPECT_EQ(compVals == c, param.expectedEqual);
    EXPECT_EQ(compVals != c, param.expectedNotEqual);
    EXPECT_EQ(compVals < c,  param.expectedLessThan);
    EXPECT_EQ(compVals <= c, param.expectedLessOrEqualThan);
    EXPECT_EQ(compVals > c,  param.expectedGreaterThan);
    EXPECT_EQ(compVals >= c, param.expectedGreaterOrEqualThan);
}

std::vector<vs_test::CompBoolOpParam>
vs_test::CompBoolOpParam::caseAll = {
        {
            "equal", { 8, 0, 2, 5, 2, 6 },
            /* = */ true, false, /**/ false, true, /**/ false, true
        }, {
            "shorter", { 8, 0, 2, 5, 2 },
            /* = */ false, true, /**/ true, true, /**/ false, false
        }, {
            "longer", { 8, 0, 2, 5, 2, 6, 8 },
            /* = */ false, true, /**/ false, false, /**/ true, true
        }, {
            "lessEqLen", { 8, 0, 2, 4, 2, 6 },
            /* = */ false, true, /**/ true, true, /**/ false, false
        }, {
            "greaterEqLen", { 8, 0, 2, 6, 2, 6 },
            /* = */ false, true, /**/ false, false, /**/ true, true
        }, {
            "lessShorter", { 8, 0, 2, 4, 2},
            /* = */ false, true, /**/ true, true, /**/ false, false
        }, {
            "greaterShorter", { 8, 0, 2, 6, 2},
            /* = */ false, true, /**/ false, false, /**/ true, true
        }, {
            "lessLonger", { 8, 0, 2, 4, 2, 6, 8},
            /* = */ false, true, /**/ true, true, /**/ false, false
        }, {
            "greaterLonger", { 8, 0, 2, 6, 2, 6, 8 },
            /* = */ false, true, /**/ false, false, /**/ true, true
        }
};
