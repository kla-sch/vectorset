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

#include "VectorManipulation.h"


TEST_F(VectorManipulation, UnorderedPushBackCopy) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    const auto value = vs_test::TestValue(10);
    c.push_back(value);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"push_back(const Key&)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 7u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6, 10));
}

TEST_F(VectorManipulation, UnorderedPushBackMove) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    auto value = vs_test::TestValue(10);
    c.push_back(std::move(value));

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"push_back(Key&&)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 7u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6, 10));
}


TEST_F(VectorManipulation, UnorderedPopBack) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    c.pop_back();

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"pop_back()", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2));
}


TEST_F(VectorManipulation, UnorderedResizeShrink) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(4);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"resize(size_type)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5));
}


TEST_F(VectorManipulation, UnorderedResizeExpand) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(8);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"resize(size_type)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 8u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6, 0, 0));
}


TEST_F(VectorManipulation, UnorderedResizeExpandValue) {
    auto &c = container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(8, vs_test::TestValue(10));

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"set_mode(vectorset_mode)", "mode-equal"},
                {"resize(size_type,const value_type&)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 8u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(8, 0, 2, 5, 2, 6, 10, 10));
}


TEST_F(VectorManipulation, OrderedPushBackCopy) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        const auto value = vs_test::TestValue(10);
        c.push_back(value);
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"push_back(const Key&)", "start"},
                {"push_back(const Key&)", "logic_error"}
            }));

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}

TEST_F(VectorManipulation, OrderedPushBackMove) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        auto value = vs_test::TestValue(10);
        c.push_back(std::move(value));
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"push_back(Key&&)", "start"},
                {"push_back(Key&&)", "logic_error"}
            }));

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(VectorManipulation, OrderedPopBack) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    c.pop_back();

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"pop_back()", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6));
}


TEST_F(VectorManipulation, OrderedResizeShrink) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    c.resize(4);

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"resize(size_type)", "start"}
            }));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6));
}


TEST_F(VectorManipulation, OrderedResizeExpand) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        c.resize(7);
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"resize(size_type)", "start"},
                {"resize(size_type)", "logic_error"}
            }));

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(VectorManipulation, OrderedResizeExpandValue) {
    auto &c = container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        c.resize(8, vs_test::TestValue(10));
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(
        vs_test::checkTrace(
            c.getDebugTrace(),
            {
                {"set_mode(vectorset_mode)", "start"},
                {"make_unique_ordered()", "start"},
                {"make_unique_ordered()", "rmIter:5(6)"},
                {"set_mode(vectorset_mode)", "mode-ordered"},
                {"resize(size_type,const value_type&)", "start"},
                {"resize(size_type,const value_type&)", "logic_error"}
            }));

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, vs_test::make_vector<int>(0, 2, 5, 6, 8));
}
