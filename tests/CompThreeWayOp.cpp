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

#include "CompThreeWayOp.h"

#if __cplusplus >= 202002L // c++20

TEST_P(CompThreeWayOp, any) {
    const auto &param = GetParam();
    const auto &c = container;

    CompThreeWayOp::Container compVals;
    std::transform(
        param.values.begin(), param.values.end(),
        inserter(compVals, compVals.end()),
        [](const int value) {
            return vs_test::TestValue(value);
        });

    EXPECT_EQ(compVals <=> c, param.expectedResult);
}


std::vector<vs_test::CompThreeWayOpParam>
vs_test::CompThreeWayOpParam::caseAll = {
    {
        "equal", { 8, 0, 2, 5, 2, 6 },
        /* = */ std::strong_ordering::equal
    }, {
        "shorter", { 8, 0, 2, 5, 2 },
        /* = */ std::strong_ordering::less
    }, {
        "longer", { 8, 0, 2, 5, 2, 6, 8 },
        /* = */ std::strong_ordering::greater
    }, {
        "lessEqLen", { 8, 0, 2, 4, 2, 6 },
        /* = */ std::strong_ordering::less
    }, {
        "greaterEqLen", { 8, 0, 2, 6, 2, 6 },
        /* = */ std::strong_ordering::greater
    }, {
        "lessShorter", { 8, 0, 2, 4, 2},
        /* = */ std::strong_ordering::less
    }, {
        "greaterShorter", { 8, 0, 2, 6, 2},
        /* = */ std::strong_ordering::greater
    }, {
        "lessLonger", { 8, 0, 2, 4, 2, 6, 8},
        /* = */ std::strong_ordering::less
    }, {
        "greaterLonger", { 8, 0, 2, 6, 2, 6, 8 },
        /* = */ std::strong_ordering::greater
    }
};


#endif /* __cplusplus >= 202002L */
