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

#ifndef VECTOR_SET_TEST_COMP_BOOL_OP_H
#define VECTOR_SET_TEST_COMP_BOOL_OP_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "ContainerBase.h"
#include "TestUtils.h"

#include "gtest/gtest.h"
// Boolean Compare Operators
////////////////////////////////////////////////////////////////////////


namespace vs_test {

    struct CompBoolOpParam {
        std::string name;

        std::vector<int> values;

        bool expectedEqual;
        bool expectedNotEqual;
        bool expectedLessThan;
        bool expectedLessOrEqualThan;
        bool expectedGreaterThan;
        bool expectedGreaterOrEqualThan;

        friend std::ostream&
        operator<<(std::ostream& os, const CompBoolOpParam& param) {
            return
                os << std::boolalpha
                   << "{ name=\"" << param.name << "\""
                   << " || input:"
                   << " values=" << param.values
                   << " || expected:"
                   << " equal=" << param.expectedEqual
                   << ", not equal=" << param.expectedNotEqual
                   << ", less than=" << param.expectedLessThan
                   << ", less or equal than=" << param.expectedLessOrEqualThan
                   << ", greater than=" << param.expectedGreaterThan
                   << ", greater or equal than="
                   << param.expectedGreaterOrEqualThan
                   << " }";
        }

        // Test parameters:
        static std::vector<CompBoolOpParam> caseAll;
    };

} // namespace vs_test


class CompBoolOp
    : public vs_test::ContainerBase
    , public testing::WithParamInterface<vs_test::CompBoolOpParam>
{
};


#endif /* VECTOR_SET_TEST_COMP_BOOL_OP_H */
