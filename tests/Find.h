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

#ifndef VECTOR_SET_TEST_FIND_H
#define VECTOR_SET_TEST_FIND_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "ContainerBase.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

// Find methods:
// find(const Key& key)
// find(const Key& key) const
// template<class K> find(const K& x)
// template<class K> find(const K& x) const
////////////////////////////////////////////////////////////////////////

namespace vs_test {
    struct FindParam {
        std::string name;

        int value;
        bool ordered;

        int expectedOffset;

        _Vectorset_Trace traceKey;       // for Test "Key"
        _Vectorset_Trace traceConstKey;  // for Test "ConstKey"
        _Vectorset_Trace traceMisc;      // for Test "Misc"
        _Vectorset_Trace traceConstMisc; // for Test "ConstMisc"


        friend std::ostream&
        operator<<(std::ostream& os, const FindParam& param) {
            return
                os << std::boolalpha
                   << "{ name=\"" << param.name << "\""
                   << " || input:"
                   << " value=" << param.value
                   << ", ordered=" << param.ordered
                   << " || expected:"
                   << " offset=" << param.expectedOffset
                   << " || traceKey: " << param.traceKey
                   << " || traceConstKey: " << param.traceConstKey
                   << " || traceMisc: " << param.traceMisc
                   << " || traceConstMisc: " << param.traceConstMisc
                   << " }";
        }

        // Test parameters:
        static std::vector<FindParam> caseUnordered;
        static std::vector<FindParam> caseOrdered;
    };
} // namespace vs_test

class Find
    : public vs_test::ContainerBase
    , public testing::WithParamInterface<vs_test::FindParam>
{
};

#endif /* VECTOR_SET_TEST_FIND_H */
