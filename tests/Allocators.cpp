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

#include "Allocators.h"

#include "TestUtils.h"

/// Flag: allocate/deallocate has been used.
static bool testAllocUsed = false;


/**
 * Allocator class operator for class vs_test::TestValue
 */
template<class T>
struct TestAlloc : public std::allocator<T> {

    template<class U>
    struct rebind {
        typedef TestAlloc<U> other;
    };

#if __cplusplus <= 201703L
    T *
    allocate(size_t n, const void * hint = 0) {
        testAllocUsed = true;

        return std::allocator<T>::allocate(n, hint);
    }
#else
    T *
    allocate(size_t n) {
        testAllocUsed = true;
        return std::allocator<T>::allocate(n);
    }
#endif

    void
    deallocate(T *p, std::size_t n) {
        std::allocator<T>::deallocate(p, n);
    }

};


TEST_F(Allocators, ClassAlloc) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };

    testAllocUsed = false;

    vectorset<
        vs_test::TestValue,
        std::less<vs_test::TestValue>,
        TestAlloc<vs_test::TestValue>> c(values.begin(), values.end());

    EXPECT_EQ(c, values);
    EXPECT_TRUE(testAllocUsed);
}
