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

#include "ModeSwitch.h"

TEST_F(ModeSwitch, Ordered) {
    auto &c = container;

    c.set_mode(vectorset_mode::unique_ordered);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(ModeSwitch, OrderedBeforeUnordered) {
    auto &c = container;

    c.set_mode(vectorset_mode::unique_ordered);
    c.set_mode(vectorset_mode::unordered);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-unordered"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}

TEST_F(ModeSwitch, 2xOrdered) {
    auto &c = container;

    c.set_mode(vectorset_mode::unique_ordered);
    c.set_mode(vectorset_mode::unique_ordered);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}

TEST_F(ModeSwitch, 2xUnordered) {
    auto &c = container;

    c.set_mode(vectorset_mode::unordered);
    c.set_mode(vectorset_mode::unordered);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6));
}
