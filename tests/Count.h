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

#ifndef VECTOR_SET_TEST_COUNT_H
#define VECTOR_SET_TEST_COUNT_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "ContainerBase.h"
#include "TestUtils.h"

#include "gtest/gtest.h"


// Count methods:
// count(const Key& key) const
// template<class K> count(const K& x) const
////////////////////////////////////////////////////////////////////////

namespace vs_test {
    struct CountParam {
        std::string name;

        int value;
        bool ordered;

        size_t expectedCount;

        _Vectorset_Trace traceKey;  // for Test "Key"
        _Vectorset_Trace traceMisc; // for Test "Misc"

        friend std::ostream&
        operator<<(std::ostream& os, const CountParam& param) {
            return
                os << std::boolalpha
                   << "{ name=\"" << param.name << "\""
                   << " || input:"
                   << " value=" << param.value
                   << ", ordered=" << param.ordered
                   << " || expected:"
                   << " count=" << param.expectedCount
                   << " || traceKey: " << param.traceKey
                   << " || traceMisc: " << param.traceMisc
                   << " }";
        }

        // Test parameters:
        static std::vector<CountParam> caseUnordered;
        static std::vector<CountParam> caseOrdered;
    };

} // namespace vs_test


class Count
    : public vs_test::ContainerBase
    , public testing::WithParamInterface<vs_test::CountParam>
{
};



#endif /* VECTOR_SET_TEST_COUNT_H */
