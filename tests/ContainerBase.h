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

#ifndef VECTOR_SET_TEST_CONTAINER_BASE_H
#define VECTOR_SET_TEST_CONTAINER_BASE_H 1

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"
#include "TestUtils.h"

#include "gtest/gtest.h"

namespace vs_test {

    /**
     * Base Container for all tests.
     */
    class ContainerBase : public testing::Test {
    public:
        using Container = vectorset<vs_test::TestValue>;

        Container container;

        void
            SetUp() override {
            container =
                vs_test::make_vectorset<vs_test::TestValue>(8, 0, 2, 5, 2, 6);
            // ordered: { 0, 2, 5, 6, 8 }
            container.clearDebugTrace();
        }
    };
} // namespace vs_test

#endif /* VECTOR_SET_TEST_CONTAINER_BASE_H */
