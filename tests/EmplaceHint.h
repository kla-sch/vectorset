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

#ifndef VECTOR_SET_TEST_EMPLACE_HINT_H
#define VECTOR_SET_TEST_EMPLACE_HINT_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "ContainerBase.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

// Emplace with hint method:
// emplace_hint(const_iterator hint, Args&&... args)
////////////////////////////////////////////////////////////////////////

namespace vs_test {
    struct EmplaceHintParam {
        std::string name;

        int value;
        int posOffset;
        bool ordered;

        size_t expectedSize;
        bool expectNoCopiedElem;
        std::vector<int> expectedResult;

        _Vectorset_Trace trace;

        friend std::ostream&
        operator<<(std::ostream& os, const EmplaceHintParam& param) {
            return
                os << std::boolalpha
                   << "{ name=\"" << param.name << "\""
                   << " || input:"
                   << " value=" << param.value
                   << ", posOffset=" << param.posOffset
                   << ", ordered=" << param.ordered
                   << " || expected:"
                   << " size=" << param.expectedSize
                   << ", noCopiedElem=" << param.expectNoCopiedElem
                   << ", result=" << param.expectedResult
                   << " || trace: " << param.trace
                   << " }";
        }

        // Test parameters:
        static std::vector<EmplaceHintParam> caseUnorderedPosBegin;
        static std::vector<EmplaceHintParam> caseUnorderedPos2;
        static std::vector<EmplaceHintParam> caseUnorderedPosEnd;
        static std::vector<EmplaceHintParam> caseUnorderedPosMatch;
        static std::vector<EmplaceHintParam> caseOrderedPosBegin;
        static std::vector<EmplaceHintParam> caseOrderedPos2;
        static std::vector<EmplaceHintParam> caseOrderedPosEnd;
        static std::vector<EmplaceHintParam> caseOrderedPosMatchMiddle;
        static std::vector<EmplaceHintParam> caseOrderedPosMatchBegin;
        static std::vector<EmplaceHintParam> caseOrderedPosMatchEnd;

    };
} // namespace vs_test

class EmplaceHint
    : public vs_test::ContainerBase
    , public testing::WithParamInterface<vs_test::EmplaceHintParam>
{
};


#endif /* VECTOR_SET_TEST_EMPLACE_HINT_H */
