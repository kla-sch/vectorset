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

#ifndef VECTOR_SET_TEST_RANGE_BOUND_H
#define VECTOR_SET_TEST_RANGE_BOUND_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "ContainerBase.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

// Range / Bound lookup:
// equal_range(const Key& key)
// equal_range(const Key& key) const
// template<class K> equal_range(const K& x) (only >= C++14)
// template<class K> equal_range(const K& x) const (only >= C++14)
// lower_bound(const Key& key)
// lower_bound(const Key& key) const
// template<class K> lower_bound(const K& x) (only >= C++14)
// template<class K> lower_bound(const K& x) const (only >= C++14)
// upper_bound(const Key& key)
// upper_bound(const Key& key) const
// template<class K> upper_bound(const K& x) (only >= C++14)
// template<class K> upper_bound(const K& x) const (only >= C++14)
////////////////////////////////////////////////////////////////////////

#define RANGE_BOUND_WITH_MISC_VALUE (__cplusplus >= 201402L)

namespace vs_test {
    struct RangeBoundParam {
        enum class func : char { equal_range, lower_bound, upper_bound };

        std::string name;

        int value;
        bool ordered;
        func function;

        int expectedLowerBoundOffset;
        int expectedUpperBoundOffset;

        _Vectorset_Trace traceKey;       // for Test "Key"
        _Vectorset_Trace traceConstKey;  // for Test "ConstKey"
#if RANGE_BOUND_WITH_MISC_VALUE
        _Vectorset_Trace traceMisc;      // for Test "Misc"
        _Vectorset_Trace traceConstMisc; // for Test "ConstMisc"
#endif

        friend std::ostream&
        operator<<(std::ostream& os, const RangeBoundParam& param) {
            const char *fnName = nullptr;
            switch (param.function) {
            case RangeBoundParam::func::equal_range:
                fnName = "equal_range";
                break;
            case RangeBoundParam::func::lower_bound:
                fnName = "lower_bound";
                break;
            case RangeBoundParam::func::upper_bound:
                fnName = "upper_bound";
                break;
            }

            return
                os << std::boolalpha
                   << "{ name=\"" << param.name << "\""
                   << " || input:"
                   << " value=" << param.value
                   << ", ordered=" << param.ordered
                   << ", function=" << fnName
                   << " || expected:"
                   << " lowerBoundOffset=" << param.expectedLowerBoundOffset
                   << ", upperBoundOffset=" << param.expectedUpperBoundOffset
                   << " || traceKey: " << param.traceKey
                   << " || traceConstKey: " << param.traceConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
                   << " || traceMisc: " << param.traceMisc
                   << " || traceConstMisc: " << param.traceConstMisc
#endif
                   << " }";
        }

        // Test parameters:
        static std::vector<RangeBoundParam> caseUnorderedEqualRange;
        static std::vector<RangeBoundParam> caseOrderedEqualRange;
        static std::vector<RangeBoundParam> caseUnorderedLowerBound;
        static std::vector<RangeBoundParam> caseOrderedLowerBound;
        static std::vector<RangeBoundParam> caseUnorderedUpperBound;
        static std::vector<RangeBoundParam> caseOrderedUpperBound;
    };

} // namespace vs_test

class RangeBound
    : public vs_test::ContainerBase
    , public testing::WithParamInterface<vs_test::RangeBoundParam>
{
};


#endif /* VECTOR_SET_TEST_RANGE_BOUND_H */
