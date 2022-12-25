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

#include "Constructor.h"

TEST_F(Constructor, Default) {
    vectorset<vs_test::TestValue> c;

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"vectorset()", "construct"}
            }));

    EXPECT_TRUE(c.empty());
    EXPECT_EQ(c.size(), 0u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
}

TEST_F(Constructor, InitializerList) {
    vectorset<vs_test::TestValue> c = { {8}, {0}, {2}, {5}, {2}, {6} };

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {
                    "vectorset("
                        "std::initializer_list<value_type>,"
                        "const Compare&,const Allocator&)",
                    "construct"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6));
}

TEST_F(Constructor, Iterator) {
    auto data = vs_test::make_vector<vs_test::TestValue>(8, 0, 2, 5, 2, 6);

    vectorset<vs_test::TestValue> c(data.begin(), data.end());

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {
                    "vectorset("
                        "InputIt,InputIt,const Compare&,const Allocator&)",
                    "construct"
                }
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6));
}
