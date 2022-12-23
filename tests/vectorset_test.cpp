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

#include "vectorset.h"
#include <string>
#include <iostream>

#include "gtest/gtest.h"


/**
 * Class for test operatons.
 */
struct TestValue {

    /// Global debug flag to trace constructor and assign operations.
    static bool debug;

    /**
     * Class to set/reset debug flag in scope.
     */
    struct DebugScope {
        DebugScope() {
            debug = true;
        }

        ~DebugScope() {
            debug = false;
        }
    };


    /**
     * Write debug message, if debug flag is enabled.
     *
     * @param msg Message to write.
     */
    void
    debugMsg(const char *msg) {
        if (debug) {
            std::cout << msg << std::endl;
        }
    }


    /// Value of instance.
    int val;

    /// Flag: instance has been copied.
    mutable bool copied;

    /// Flag: operator< has been used.
    static bool lessUsed;


    TestValue() : val(0), copied(false) {
        debugMsg("Default construct");
    };

    TestValue(int val) noexcept : val(val), copied(false) {
        debugMsg("Init construct");
    }

    TestValue(const TestValue &other) noexcept : val(other.val), copied(true) {
        debugMsg("Copy construct");
    }

    TestValue(TestValue &&other) noexcept
        : val(other.val), copied(other.copied)
    {
        debugMsg("Move construct");
    }

    void
    operator=(const TestValue &other) noexcept {
        debugMsg("Copy assign");
        val = other.val;
        copied = true;
    }

    void
    operator=(TestValue &&other) noexcept {
        debugMsg("Move assign");
        val = other.val;
        copied = other.copied;
    }

    bool
    operator<(const TestValue &other) const noexcept {
        lessUsed = true;
        return (val < other.val);
    }

    bool
    operator<(const int &other) const noexcept {
        lessUsed = true;
        return (val < other);
    }

    bool
    operator==(const TestValue &other) const noexcept {
        return (val == other.val);
    }

    bool
    operator==(const int &other) const noexcept {
        return (val == other);
    }
};

/// Debug flag: disabled by default. Use TestValue::DebugScope to enable.
bool TestValue::debug = false;

/// Flag: operator< has been used.
bool TestValue::lessUsed = false;


// Miscellaneous class for lookup tests.
struct MiscInt {
    int val;

    MiscInt() = delete;
    MiscInt(int value) : val(value) {}
};

// TestValue < MiscInt
bool
operator<(const TestValue &cpt, const MiscInt &mi) {
    return (cpt.val < mi.val);
}

// MiscInt < TestValue
bool
operator<(const MiscInt &mi, const TestValue &cpt) {
    return (mi.val < cpt.val);
}


/**
 * Write values of container with TestValue instances.
 *
 * @param c Container with instances.
 */
static std::ostream&
operator<<(std::ostream& os, const vectorset<TestValue> &c) {
    os << "{ ";
    bool first = true;
    for (const auto &item : c) {
        if (first) {
            first = false;
        }
        else {
            os << ", ";
        }
        os << item.val;
    }

    return os << " }";
}

void
PrintTo(const vectorset<TestValue> &c, std::ostream* os) {
    *os << c;
}


/**
 *  Write values of vector with int.
 *
 * @param c vector<int> with values.
 */
static std::ostream&
operator<<(std::ostream& os, const std::vector<int> &c) {
    os << "{ ";
    bool first = true;
    for (auto val : c) {
        if (first) {
            first = false;
        }
        else {
            os << ", ";
        }
        os << val;
    }

    return os << " }";
}


#if __cplusplus >= 201703L // compiler >= C++17

/**
 * Creates vectorset of type T.
 *
 * @param args earch argument is an value of type T.
 */
template <typename T, typename... Args>
vectorset<T> make_vectorset(Args&&... args)
{
   vectorset<T> result;
   result.reserve(sizeof...(Args));
   (result.insert(std::forward<Args>(args)), ...);
   return result;
}

#else // compiler < C++17

// generation helper for make_vectorset
template<typename T>
void
make_vectorset_gen(vectorset<T> *result, T&& val) {
    result->push_back(std::forward<T>(val));
}

// generation helper for make_vectorset
template<typename T, typename ...Next>
void
make_vectorset_gen(vectorset<T> *result, T&& val, Next&& ...next) {
    result->push_back(std::forward<T>(val));
    make_vectorset_gen<T>(result, std::forward<Next>(next)...);
}

/**
 * Creates vectorset of type T.
 *
 * @param args earch argument is an value of type T.
 */
template<typename T, typename... Args>
vectorset<T>
make_vectorset(Args&& ...args) {
    vectorset<T> result;
    result.reserve(sizeof...(args));

    make_vectorset_gen<T>(&result, std::forward<Args>(args)...);

    return result;
}
#endif // compiler < C++17


#if __cplusplus >= 201703L // compiler > C++17

/**
 * Creates vector of type T.
 *
 * @param args earch argument is an value of type T.
 *
 * @return created vector.
 */
template <typename T, typename... Args>
std::vector<T> make_vector(Args&&... args)
{
    std::vector<T> result;
    result.reserve(sizeof...(Args));
    (result.push_back(std::forward<Args>(args)), ...);
    return result;
}

#else // __cplusplus < 201703L

// generation helper for make_vector
template<typename T>
void
make_vector_gen(std::vector<T> *result, T&& val) {
    result->push_back(std::forward<T>(val));
}

// generation helper for make_vector
template<typename T, typename... Next>
void
make_vector_gen(std::vector<T> *result, T&& val, Next&&... next) {
    result->push_back(std::forward<T>(val));
    make_vector_gen<T>(result, std::forward<Next>(next)...);
}

/**
 * Creates vector of type T.
 *
 * @param args earch argument is an value of type T.
 */
template<typename T, typename... Args>
std::vector<T>
make_vector(Args&& ...args) {
    std::vector<T> result;
    result.reserve(sizeof...(args));
    make_vector_gen<T>(&result, std::forward<Args>(args)...);

    return result;
}

#endif // __cplusplus < 201703L


/**
 * Check, that none of container elements has been copied.
 */
static bool
hasNoCopiedElem(const vectorset<TestValue> &c) {
    return
        std::none_of(
            c.begin(), c.end(),
            [](const TestValue &item) {
                return item.copied;
            });
}

#if 0
/**
 * Check, that none of container elements has been compared with operator<().
 */
static bool
noLessCompUsed(const vectorset<TestValue> &c) {
    return
        std::none_of(
            c.begin(), c.end(),
            [](const TestValue &item) {
                return item.lessUsed;
            });
}
#endif


/**
 * Compare result container.
 *
 * @param c Result container.
 * @param res expected result as vector.
 *
 * @return true, if the container contains the exprected result.
 */
template<class Compare, class Allocator>
static bool
operator==(
    const vectorset<TestValue, Compare, Allocator> &c,
    const std::vector<int> &res)
{
    return (
        c.size() == res.size()
        && std::equal(c.begin(), c.end(), res.begin()));
}


// Constructors
////////////////////////////////////////////////////////////////////////

TEST(Constructor, Default) {
    vectorset<TestValue> c;

    EXPECT_TRUE(c.empty());
    EXPECT_EQ(c.size(), 0u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
}

TEST(Constructor, InitializerList) {
    vectorset<TestValue> c = { {8}, {0}, {2}, {5}, {2}, {6} };

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6));
}

TEST(Constructor, Iterator) {
    auto data = make_vector<TestValue>(8, 0, 2, 5, 2, 6);

    vectorset<TestValue> c(data.begin(), data.end());

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6));
}

// Basic test class
////////////////////////////////////////////////////////////////////////

class ContainerBase : public testing::Test
{
public:
    using Container = vectorset<TestValue>;

    static Container container;

    void
    SetUp() override {
        container = make_vectorset<TestValue>(8, 0, 2, 5, 2, 6);
        // ordered: { 0, 2, 5, 6, 8 }
    }
};

vectorset<TestValue> ContainerBase::container;


/// Get name of parameter set.
template<class ParamStruct>
static std::string
paramSetName(const testing::TestParamInfo<ParamStruct> &info) {
    return info.param.name;
};


// Mode switch
////////////////////////////////////////////////////////////////////////

class ModeSwitch
    : public ContainerBase
{
};

TEST_F(ModeSwitch, Ordered) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(
        c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(ModeSwitch, OrderedBeforeUnordered) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);
    c.set_mode(vectorset_mode::unordered);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(
        c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}


// Copy & Move constructor.
////////////////////////////////////////////////////////////////////////


TEST(ConstructorCopyMove, Copy) {
    auto data = make_vectorset<TestValue>(8, 0, 2, 5, 2, 6);

    auto c = data;

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6));
}

TEST(ConstructorCopyMove, CopyOrdered) {
    auto data = make_vectorset<TestValue>(8, 0, 2, 5, 2, 6);
    data.set_mode(vectorset_mode::unique_ordered);

    auto c = data;

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}


TEST(ConstructorCopyMove, Move) {
    auto data = make_vectorset<TestValue>(8, 0, 2, 5, 2, 6);

    auto c = std::move(data);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 6u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6));
}


TEST(ConstructorCopyMove, MoveOrdered) {
    auto data = make_vectorset<TestValue>(8, 0, 2, 5, 2, 6);
    data.set_mode(vectorset_mode::unique_ordered);

    auto c = std::move(data);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}



// Plain insert methods:
// insert(const value_type &)
// insert(value_type &&)
////////////////////////////////////////////////////////////////////////

struct InsertParam {
    std::string name;

    int value;
    bool ordered;
    bool move;

    size_t expectedSize;
    bool expectInsert;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const InsertParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " value=" << param.value
               << ", ordered=" << param.ordered
               << ", move=" << param.move
               << " || expected:"
               << " size=" << param.expectedSize
               << ", inserts=" << param.expectInsert
               << ", noCopiedElem=" << param.expectNoCopiedElem
               << ", result=" << param.expectedResult
               << " }";
    }
};



class Insert
    : public ContainerBase
    , public testing::WithParamInterface<InsertParam>
{
};


TEST_P(Insert, Basic) {
    const auto &param = GetParam();
    auto &c = Insert::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    if (param.move) {
        auto value = TestValue(param.value);
        auto result = c.insert(std::move(value));

        EXPECT_EQ(result.second, param.expectInsert);
        EXPECT_EQ(*result.first, param.value);
    }
    else {
        const auto value = TestValue(param.value);
        auto result = c.insert(value);

        EXPECT_EQ(result.second, param.expectInsert);
        EXPECT_EQ(*result.first, param.value);
    }

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, Insert,
    testing::Values(
        InsertParam{
            "basic", 3, false, false,
            /* = */ 7, true, false, { 8, 0, 2, 5, 2, 6, 3 }},
        InsertParam{
            "basicMove", 3, false, true,
            /* = */ 7, true, true,  { 8, 0, 2, 5, 2, 6, 3 }},
        InsertParam{
            "duplicate", 2, false, false,
            /* = */ 7, true, false, { 8, 0, 2, 5, 2, 6, 2 }},
        InsertParam{
            "duplicateMove", 2, false, true,
            /* = */ 7, true, true,  { 8, 0, 2, 5, 2, 6, 2 }}
        ),
    paramSetName<InsertParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, Insert,
    testing::Values(
        InsertParam{
            "basic", 3, true, false,
            /* = */ 6, true, false, { 0, 2, 3, 5, 6, 8 }},
        InsertParam{
            "basicMove", 3, true, true,
            /* = */ 6, true, true,  { 0, 2, 3, 5, 6, 8 }},
        InsertParam{
            "begin", -1, true, false,
            /* = */ 6, true, false, { -1, 0, 2, 5, 6, 8 }},
        InsertParam{
            "beginMove", -1, true, true,
            /* = */ 6, true, true,  { -1, 0, 2, 5, 6, 8 }},
        InsertParam{
            "end", 10, true, false,
            /* = */ 6, true, false, { 0, 2, 5, 6, 8, 10 }},
        InsertParam{
            "endMove", 10, true, true,
            /* = */ 6, true, true,  { 0, 2, 5, 6, 8, 10 }},
        InsertParam{
            "duplicate", 2, true, false,
            /* = */ 5, false, true, { 0, 2, 5, 6, 8 }},
        InsertParam{
            "duplicateMove", 2, true, true,
            /* = */ 5, false, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertParam>);


// insert at position methods:
// insert(const_iterator pos, const value_type &)
// insert(const_iterator pos, value_type &&)
////////////////////////////////////////////////////////////////////////

struct InsertPosParam {
    std::string name;

    int value;
    int posOffset;
    bool ordered;
    bool move;

    size_t expectedSize;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const InsertPosParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " value=" << param.value
               << ", posOffset=" << param.posOffset
               << ", ordered=" << param.ordered
               << ", move=" << param.move
               << " || expected:"
               << " size=" << param.expectedSize
               << ", noCopiedElem=" << param.expectNoCopiedElem
               << ", result=" << param.expectedResult
               << " }";
    }
};

class InsertPos
    : public ContainerBase
    , public testing::WithParamInterface<InsertPosParam>
{
};

TEST_P(InsertPos, Basic) {
    const auto &param = GetParam();
    auto &c = InsertPos::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto pos_iter = (
        param.posOffset >=0
        ? c.begin() + param.posOffset
        : c.end());

    auto value = TestValue(param.value);
    if (param.move) {
        auto result = c.insert(pos_iter, std::move(value));

        EXPECT_EQ(*result, param.value);
    }
    else {
        auto result = c.insert(pos_iter, value);

        EXPECT_EQ(*result, param.value);
    }

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    unorderedPosBegin, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, 0, false, false,
            /* = */ 7, false, { 3, 8, 0, 2, 5, 2, 6 }},
        InsertPosParam{
            "basicMove", 3, 0, false, true,
            /* = */ 7, true,  { 3, 8, 0, 2, 5, 2, 6 }},
        InsertPosParam{
            "duplicate", 2, 0, false, false,
            /* = */ 7, false, { 2, 8, 0, 2, 5, 2, 6 }},
        InsertPosParam{
            "duplicateMove", 2, 0, false, true,
            /* = */ 7, true,  { 2, 8, 0, 2, 5, 2, 6 }}
        ),
    paramSetName<InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPos2, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, 2, false, false,
            /* = */ 7, false, { 8, 0, 3, 2, 5, 2, 6 }},
        InsertPosParam{
            "basicMove", 3, 2, false, true,
            /* = */ 7, true,  { 8, 0, 3, 2, 5, 2, 6 }},
        InsertPosParam{
            "duplicate", 2, 2, false, false,
            /* = */ 7, false, { 8, 0, 2, 2, 5, 2, 6 }},
        InsertPosParam{
            "duplicateMove", 2, 2, false, true,
            /* = */ 7, true,  { 8, 0, 2, 2, 5, 2, 6 }}
        ),
    paramSetName<InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosEnd, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, -1, false, false,
            /* = */ 7, false, { 8, 0, 2, 5, 2, 6, 3 }},
        InsertPosParam{
            "basicMove", 3, -1, false, true,
            /* = */ 7, true,  { 8, 0, 2, 5, 2, 6, 3 }},
        InsertPosParam{
            "duplicate", 2, -1, false, false,
            /* = */ 7, false, { 8, 0, 2, 5, 2, 6, 2 }},
        InsertPosParam{
            "duplicateMove", 2, -1, false, true,
            /* = */ 7, true,  { 8, 0, 2, 5, 2, 6, 2 }}
        ),
    paramSetName<InsertPosParam>);


INSTANTIATE_TEST_SUITE_P(
    unorderedPosMatch, InsertPos,
    testing::Values(
        InsertPosParam{
            "duplicate", 2, 2, false, false,
            /* = */ 7, false, { 8, 0, 2, 2, 5, 2, 6 }},
        InsertPosParam{
            "duplicateMove", 2, 2, false, true,
            /* = */ 7, true,  { 8, 0, 2, 2, 5, 2, 6 }}
        ),
    paramSetName<InsertPosParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosBegin, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, 0, true, false,
            /* = */ 6, false, { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "basicMove", 3, 0, true, true,
            /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "duplicate", 2, 0, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 2, 0, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPos2, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 1, 2, true, false,
            /* = */ 6, false, { 0, 1, 2, 5, 6, 8 }},
        InsertPosParam{
            "basicMove", 1, 2, true, true,
            /* = */ 6, true,  { 0, 1, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicate", 2, 2, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 2, 2, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosEnd, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, -1, true, false,
            /* = */ 6, false, { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "basicMove", 3, -1, true, true,
            /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "duplicate", 2, -1, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 2, -1, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchMiddle, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 3, 2, true, false,
            /* = */ 6, false, { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "basicMove", 3, 2, true, true,
            /* = */ 6, true,  { 0, 2, 3, 5, 6, 8 }},
        InsertPosParam{
            "duplicate", 2, 1, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 2, 1, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchBegin, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", -1, 0, true, false,
            /* = */ 6, false, { -1, 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "basicMove",-1, 0, true, true,
            /* = */ 6, true,  { -1, 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicate", 0, 0, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 0, 0, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchEnd, InsertPos,
    testing::Values(
        InsertPosParam{
            "basic", 10, -1, true, false,
            /* = */ 6, false, { 0, 2, 5, 6, 8, 10 }},
        InsertPosParam{
            "basicMove", 10, -1, true, true,
            /* = */ 6, true,  { 0, 2, 5, 6, 8, 10 }},
        InsertPosParam{
            "duplicate", 8, 4, true, false,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }},
        InsertPosParam{
            "duplicateMove", 8, 4, true, true,
            /* = */ 5, true,  { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertPosParam>);


// insert by iterators:
// insert(InputIter first, InputIter last)
////////////////////////////////////////////////////////////////////////

struct InsertByIterParam {
    std::string name;

    std::vector<int> values;
    bool ordered;

    size_t expectedSize;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const InsertByIterParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " values=" << param.values
               << ", ordered=" << param.ordered
               << " || expected:"
               << " size=" << param.expectedSize
               << ", noCopiedElem=" << param.expectNoCopiedElem
               << ", result=" << param.expectedResult
               << " }";
    }
};

class InsertByIter
    : public ContainerBase
    , public testing::WithParamInterface<InsertByIterParam>
{
};


TEST_P(InsertByIter, Basic) {
    const auto &param = GetParam();
    auto &c = InsertByIter::container;

    InsertByIter::Container newValues;
    newValues.reserve(param.values.size());

    std::transform(
        param.values.begin(), param.values.end(),
        inserter(newValues, newValues.end()),
        [](const int value) {
            return TestValue(value);
        });

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    c.insert(newValues.begin(), newValues.end());

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}


INSTANTIATE_TEST_SUITE_P(
    unordered, InsertByIter,
    testing::Values(
        InsertByIterParam{
            "basic", { -1, 2, 5, 8 }, false,
            /* = */ 10, false, { 8, 0, 2, 5, 2, 6, -1, 2, 5, 8 }}
        ),
    paramSetName<InsertByIterParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, InsertByIter,
    testing::Values(
        InsertByIterParam{
            "basic", { -1, 2, 5, 8 }, true,
            /* = */ 6, false, { -1, 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertByIterParam>);


// insert by initializer_list:
// insert(std::initializer_list<value_type> ilist)
////////////////////////////////////////////////////////////////////////

struct InsertByInitListParam {
    std::string name;

    bool ordered;

    size_t expectedSize;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const InsertByInitListParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " ordered=" << param.ordered
               << " || expected:"
               << " size=" << param.expectedSize
               << ", noCopiedElem=" << param.expectNoCopiedElem
               << ", result=" << param.expectedResult
               << " }";
    }
};

class InsertByInitList
    : public ContainerBase
    , public testing::WithParamInterface<InsertByInitListParam>
{
};


TEST_P(InsertByInitList, Basic) {
    const auto &param = GetParam();
    auto &c = InsertByInitList::container;

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    c.insert(
        {
            TestValue(-1),
            TestValue(2),
            TestValue(5),
            TestValue(8),
        });

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}


INSTANTIATE_TEST_SUITE_P(
    unordered, InsertByInitList,
    testing::Values(
        InsertByInitListParam{
            "basic", false,
            /* = */ 10, false, { 8, 0, 2, 5, 2, 6, -1, 2, 5, 8 }}
        ),
    paramSetName<InsertByInitListParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, InsertByInitList,
    testing::Values(
        InsertByInitListParam{
            "basic", true,
            /* = */ 6, false, { -1, 0, 2, 5, 6, 8 }}
        ),
    paramSetName<InsertByInitListParam>);


// Plain emplace method:
// emplace(Args&&... args)
////////////////////////////////////////////////////////////////////////
struct EmplaceParam {
    std::string name;

    int value;
    bool ordered;

    size_t expectedSize;
    bool expectInsert;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const EmplaceParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " value=" << param.value
               << ", ordered=" << param.ordered
               << " || expected:"
               << " size=" << param.expectedSize
               << ", inserts=" << param.expectInsert
               << ", noCopiedElem=" << param.expectNoCopiedElem
               << ", result=" << param.expectedResult
               << " }";
    }
};

class Emplace
    : public ContainerBase
    , public testing::WithParamInterface<EmplaceParam>
{
};


TEST_P(Emplace, Basic) {
    const auto &param = GetParam();
    auto &c = Insert::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto result = c.emplace(param.value);

    EXPECT_EQ(result.second, param.expectInsert);
    EXPECT_EQ(*result.first, param.value);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, Emplace,
    testing::Values(
        EmplaceParam{
            "basic", 3, false,
            /* = */ 7, true, true, { 8, 0, 2, 5, 2, 6, 3 }},
        EmplaceParam{
            "duplicate", 2, false,
            /* = */ 7, true, true, { 8, 0, 2, 5, 2, 6, 2 }}
        ),
    paramSetName<EmplaceParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, Emplace,
    testing::Values(
        EmplaceParam{
            "basic", 3, true,
            /* = */ 6, true, true, { 0, 2, 3, 5, 6, 8 }},
        EmplaceParam{
            "begin", -1, true,
            /* = */ 6, true, true, { -1, 0, 2, 5, 6, 8 }},
        EmplaceParam{
            "end", 10, true,
            /* = */ 6, true, true, { 0, 2, 5, 6, 8, 10 }},
        EmplaceParam{
            "duplicate", 2, true,
            /* = */ 5, false, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceParam>);


// Plain emplace with hint method:
// emplace_hint(const_iterator hint, Args&&... args)
////////////////////////////////////////////////////////////////////////

struct EmplaceHintParam {
    std::string name;

    int value;
    int posOffset;
    bool ordered;

    size_t expectedSize;
    bool expectNoCopiedElem;
    std::vector<int> expectedResult;

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
               << " }";
    }
};

class EmplaceHint
    : public ContainerBase
    , public testing::WithParamInterface<EmplaceHintParam>
{
};

TEST_P(EmplaceHint, Basic) {
    const auto &param = GetParam();
    auto &c = EmplaceHint::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto pos_iter = (
        param.posOffset >=0
        ? c.begin() + param.posOffset
        : c.end());

    auto result = c.emplace_hint (pos_iter, param.value);
    EXPECT_EQ(*result, param.value);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_EQ(hasNoCopiedElem(c), param.expectNoCopiedElem);
    EXPECT_EQ(c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    unorderedPosBegin, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, 0, false,
            /* = */ 7, true, { 3, 8, 0, 2, 5, 2, 6 }},
        EmplaceHintParam{
            "duplicate", 2, 0, false,
            /* = */ 7, true, { 2, 8, 0, 2, 5, 2, 6 }}
        ),
    paramSetName<EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPos2, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, 2, false,
            /* = */ 7, true, { 8, 0, 3, 2, 5, 2, 6 }},
        EmplaceHintParam{
            "duplicate", 2, 2, false,
            /* = */ 7, true, { 8, 0, 2, 2, 5, 2, 6 }}
        ),
    paramSetName<EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosEnd, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, -1, false,
            /* = */ 7, true, { 8, 0, 2, 5, 2, 6, 3 }},
        EmplaceHintParam{
            "duplicate", 2, -1, false,
            /* = */ 7, true, { 8, 0, 2, 5, 2, 6, 2 }}
        ),
    paramSetName<EmplaceHintParam>);


INSTANTIATE_TEST_SUITE_P(
    unorderedPosMatch, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "duplicate", 2, 2, false,
            /* = */ 7, true, { 8, 0, 2, 2, 5, 2, 6 }}
        ),
    paramSetName<EmplaceHintParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosBegin, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, 0, true,
            /* = */ 6, true, { 0, 2, 3, 5, 6, 8 }},
        EmplaceHintParam{
            "duplicate", 2, 0, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPos2, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 1, 2, true,
            /* = */ 6, true, { 0, 1, 2, 5, 6, 8 }},
        EmplaceHintParam{
            "duplicate", 2, 2, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosEnd, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, -1, true,
            /* = */ 6, true, { 0, 2, 3, 5, 6, 8 }},
        EmplaceHintParam{
            "duplicate", 2, -1, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchMiddle, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 3, 2, true,
            /* = */ 6, true, { 0, 2, 3, 5, 6, 8 }},
        EmplaceHintParam{
            "duplicate", 2, 1, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchBegin, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", -1, 0, true,
            /* = */ 6, true, { -1, 0, 2, 5, 6, 8 }},
        EmplaceHintParam{
            "duplicate", 0, 0, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);


INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchEnd, EmplaceHint,
    testing::Values(
        EmplaceHintParam{
            "basic", 10, -1, true,
            /* = */ 6, true, { 0, 2, 5, 6, 8, 10 }},
        EmplaceHintParam{
            "duplicate", 8, 4, true,
            /* = */ 5, true, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EmplaceHintParam>);


// erase at position method:
// erase(const_iterator pos)
////////////////////////////////////////////////////////////////////////


struct EraseAtPosParam {
    std::string name;

    int posOffset;
    bool ordered;

    size_t expectedSize;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const EraseAtPosParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " posOffset=" << param.posOffset
               << ", ordered=" << param.ordered
               << " || expected:"
               << " size=" << param.expectedSize
               << ", result=" << param.expectedResult
               << " }";
    }

};


class EraseAtPos
    : public ContainerBase
    , public testing::WithParamInterface<EraseAtPosParam>
{
};


TEST_P(EraseAtPos, Basic) {
    const auto &param = GetParam();
    auto &c = EmplaceHint::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto pos_iter = (
        param.posOffset >=0
        ? c.begin() + param.posOffset
        : c.end());

    auto erase_iter = c.erase(pos_iter);

    auto next_iter = c.begin() + param.posOffset;
    EXPECT_EQ(erase_iter, next_iter);

    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, EraseAtPos,
    testing::Values(
        EraseAtPosParam{
            "begin", 0, false,
            /* = */ 5, { 0, 2, 5, 2, 6 }},
        EraseAtPosParam{
            "middle", 3, false,
            /* = */ 5, { 8, 0, 2, 2, 6 }},
        EraseAtPosParam{
            "end", 5, false,
            /* = */ 5, { 8, 0, 2, 5, 2 }}
        ),
    paramSetName<EraseAtPosParam>);


INSTANTIATE_TEST_SUITE_P(
    ordered, EraseAtPos,
    testing::Values(
        EraseAtPosParam{
            "begin", 0, true,
            /* = */ 4, { 2, 5, 6, 8 }},
        EraseAtPosParam{
            "middle", 2, true,
            /* = */ 4, { 0, 2, 6, 8 }},
        EraseAtPosParam{
            "end", 4, true,
            /* = */ 4, { 0, 2, 5, 6 }}
        ),
    paramSetName<EraseAtPosParam>);

// erase range method:
// erase(const_iterator first, const_iterator last)
////////////////////////////////////////////////////////////////////////

struct EraseRangeParam {
    std::string name;

    int firstOffset;
    int lastOffset;
    bool ordered;

    size_t expectedSize;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const EraseRangeParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " firstOffset=" << param.firstOffset
               << ", lastOffset=" << param.lastOffset
               << ", ordered=" << param.ordered
               << " || expected:"
               << " size=" << param.expectedSize
               << ", result=" << param.expectedResult
               << " }";
    }

};


class EraseRange
    : public ContainerBase
    , public testing::WithParamInterface<EraseRangeParam>
{
};


TEST_P(EraseRange, Basic) {
    const auto &param = GetParam();
    auto &c = EmplaceHint::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto first_iter = (
        param.firstOffset >=0
        ? c.begin() + param.firstOffset
        : c.end());

    auto last_iter = (
        param.lastOffset >=0
        ? c.begin() + param.lastOffset
        : c.end());


    auto erase_iter = c.erase(first_iter, last_iter);

    auto next_iter = c.begin() + param.firstOffset;
    EXPECT_EQ(erase_iter, next_iter);

    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}


INSTANTIATE_TEST_SUITE_P(
    unordered, EraseRange,
    testing::Values(
        EraseRangeParam{
            "begin", 0, 2, false,
            /* = */ 4, { 2, 5, 2, 6 }},
        EraseRangeParam{
            "middle", 2, 4, false,
            /* = */ 4, { 8, 0, 2, 6 }},
        EraseRangeParam{
            "end", 4, -1, false,
            /* = */ 4, { 8, 0, 2, 5 }},
        EraseRangeParam{
            "all", 0, -1, false,
            /* = */ 0, { }}
        ),
    paramSetName<EraseRangeParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, EraseRange,
    testing::Values(
        EraseRangeParam{
            "begin", 0, 2, true,
            /* = */ 3, { 5, 6, 8 }},
        EraseRangeParam{
            "middle", 2, 4, true,
            /* = */ 3, { 0, 2, 8 }},
        EraseRangeParam{
            "end", 3, -1, true,
            /* = */ 3, { 0, 2, 5 }},
        EraseRangeParam{
            "all", 0, -1, true,
            /* = */ 0, { }}
        ),
    paramSetName<EraseRangeParam>);


// erase by key method:
// erase(const Key& key)
////////////////////////////////////////////////////////////////////////

struct EraseByKeyParam {
    std::string name;

    int value;
    bool ordered;

    size_t expectedErased;
    size_t expectedSize;
    std::vector<int> expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const EraseByKeyParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " value=" << param.value
               << ", ordered=" << param.ordered
               << " || expected:"
               << " erased=" << param.expectedErased
               << " size=" << param.expectedSize
               << ", result=" << param.expectedResult
               << " }";
    }

};


class EraseByKey
    : public ContainerBase
    , public testing::WithParamInterface<EraseByKeyParam>
{
};


TEST_P(EraseByKey, Basic) {
    const auto &param = GetParam();
    auto &c = EmplaceHint::container;
    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    c.set_mode(mode);

    auto erase_cnt = c.erase(param.value);

    EXPECT_EQ(erase_cnt, param.expectedErased);
    EXPECT_EQ(c.size(), param.expectedSize);
    EXPECT_EQ(c.get_mode(), mode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedResult);
}


INSTANTIATE_TEST_SUITE_P(
    unordered, EraseByKey,
    testing::Values(
        EraseByKeyParam{
            "begin", 8, false,
            /* = */ 1, 5, { 0, 2, 5, 2, 6 }},
        EraseByKeyParam{
            "middle", 5, false,
            /* = */ 1, 5, { 8, 0, 2, 2, 6 }},
        EraseByKeyParam{
            "end", 6, false,
            /* = */ 1, 5, { 8, 0, 2, 5, 2 }},
        EraseByKeyParam{
            "multible", 2, false,
            /* = */ 2, 4, { 8, 0, 5, 6 }},
        EraseByKeyParam{
            "none", 3, false,
            /* = */ 0, 6, { 8, 0, 2, 5, 2, 6 }}
        ),
    paramSetName<EraseByKeyParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, EraseByKey,
    testing::Values(
        EraseByKeyParam{
            "begin", 0, true,
            /* = */ 1, 4, { 2, 5, 6, 8 }},
        EraseByKeyParam{
            "middle", 5, true,
            /* = */ 1, 4, { 0, 2, 6, 8 }},
        EraseByKeyParam{
            "end", 8, true,
            /* = */ 1, 4, { 0, 2, 5, 6 }},
        EraseByKeyParam{
            "none", 3, true,
            /* = */ 0, 5, { 0, 2, 5, 6, 8 }}
        ),
    paramSetName<EraseByKeyParam>);



// vector manipulation:
// push_back(const Key& value)
// push_back(Key&& value)
// pop_back()
// resize(size_type count)
// resize(size_type count, const value_type& value)
////////////////////////////////////////////////////////////////////////

class VectorManipulation : public ContainerBase {
};


TEST_F(VectorManipulation, UnorderedPushBackCopy) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    auto value = TestValue(10);
    c.push_back(value);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 7u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6, 10));
}

TEST_F(VectorManipulation, UnorderedPushBackMove) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    auto value = TestValue(10);
    c.push_back(std::move(value));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 7u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6, 10));
}


TEST_F(VectorManipulation, UnorderedPopBack) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    c.pop_back();

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2));
}


TEST_F(VectorManipulation, UnorderedResizeShrink) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(4);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5));
}


TEST_F(VectorManipulation, UnorderedResizeExpand) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(8);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 8u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6, 0, 0));
}


TEST_F(VectorManipulation, UnorderedResizeExpandValue) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unordered);

    c.resize(8, TestValue(10));

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 8u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unordered);
    EXPECT_FALSE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(8, 0, 2, 5, 2, 6, 10, 10));
}


TEST_F(VectorManipulation, OrderedPushBackCopy) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        auto value = TestValue(10);
        c.push_back(value);
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}

TEST_F(VectorManipulation, OrderedPushBackMove) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        auto value = TestValue(10);
        c.push_back(std::move(value));
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(VectorManipulation, OrderedPopBack) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    c.pop_back();

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6));
}


TEST_F(VectorManipulation, OrderedResizeShrink) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    c.resize(4);

    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 4u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6));
}


TEST_F(VectorManipulation, OrderedResizeExpand) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        c.resize(7);
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}


TEST_F(VectorManipulation, OrderedResizeExpandValue) {
    auto &c = ModeSwitch::container;
    c.set_mode(vectorset_mode::unique_ordered);

    bool gotLogicError = false;
    try {
        c.resize(8, TestValue(10));
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(gotLogicError);
    EXPECT_FALSE(c.empty());
    EXPECT_EQ(c.size(), 5u);
    EXPECT_EQ(c.get_mode(), vectorset_mode::unique_ordered);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, make_vector<int>(0, 2, 5, 6, 8));
}

// miscellaneous manipulation:
// swap(vectorset& other)
////////////////////////////////////////////////////////////////////////

struct MiscManipulationSwapParam {
    std::string name;

    bool firstOrdered;
    bool secondOrdered;
    std::vector<int> secondValues;

    std::vector<int> expectedFirstResult;
    std::vector<int> expectedSecondResult;

    friend std::ostream&
    operator<<(std::ostream& os, const MiscManipulationSwapParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " firstOrdered=" << param.firstOrdered
               << ", secondOrdered=" << param.secondOrdered
               << ", secondValues=" << param.secondValues
               << " || expected:"
               << " firstResult=" << param.expectedFirstResult
               << ", secondResult=" << param.expectedSecondResult
               << " }";
    }

};

class MiscManipulationSwap
    : public ContainerBase
    , public testing::WithParamInterface<MiscManipulationSwapParam>
{
};


TEST_P(MiscManipulationSwap, Swap) {
    const auto &param = GetParam();

    auto firstMode =
        (param.firstOrdered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    auto secondMode =
        (param.secondOrdered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    auto &c = ModeSwitch::container;
    c.set_mode(firstMode);
    auto firstSize = c.size();

    vectorset<TestValue> newC;
    std::transform(
        param.secondValues.begin(), param.secondValues.end(),
        std::inserter(newC, newC.end()),
        [] (const int value) {
            return TestValue(value);
        });

    newC.set_mode(secondMode);
    auto secondSize = newC.size();

    newC.swap(c);

    EXPECT_EQ(c.size(), secondSize);
    EXPECT_EQ(c.get_mode(), secondMode);
    EXPECT_TRUE(hasNoCopiedElem(c));
    EXPECT_EQ(c, param.expectedSecondResult);

    EXPECT_EQ(newC.size(), firstSize);
    EXPECT_EQ(newC.get_mode(), firstMode);
    EXPECT_TRUE(hasNoCopiedElem(newC));
    EXPECT_EQ(newC, param.expectedFirstResult);
}


INSTANTIATE_TEST_SUITE_P(
    ordered, MiscManipulationSwap,
    testing::Values(
        MiscManipulationSwapParam{
            "unorderedEmptyUnordered",
            false, false, {},
            /* = */ { 8, 0, 2, 5, 2, 6 }, {} },
        MiscManipulationSwapParam{
            "unorderedUnordered",
            false, false, { 7, -1, 1, 4, 1, 5 },
            /* = */ { 8, 0, 2, 5, 2, 6 }, { 7, -1, 1, 4, 1, 5 } },
        MiscManipulationSwapParam{
            "orderedEmptyUnordered",
            true, false, {},
            /* = */ { 0, 2, 5, 6, 8 }, {} },
        MiscManipulationSwapParam{
            "orderedUnordered",
            true, false, { 7, -1, 1, 4, 1, 5 },
            { 0, 2, 5, 6, 8 }, { 7, -1, 1, 4, 1, 5 } },
        MiscManipulationSwapParam{
            "unorderedOrdered",
            false, true, { 7, -1, 1, 4, 1, 5 },
            /* = */ { 8, 0, 2, 5, 2, 6 }, { -1, 1, 4, 5, 7 } },
        MiscManipulationSwapParam{
            "orderedEmptyOrdered",
            true, false, {},
            /* = */ { 0, 2, 5, 6, 8 }, {} },
        MiscManipulationSwapParam{
            "orderedOrdered",
            true, true, { 7, -1, 1, 4, 1, 5 },
            /* = */ { 0, 2, 5, 6, 8 }, { -1, 1, 4, 5, 7} }
        ),
    paramSetName<MiscManipulationSwapParam>);


// Count methods:
// count(const Key& key) const
// template<class K> count(const K& x) const
////////////////////////////////////////////////////////////////////////


struct CountParam {
    std::string name;

    int value;
    bool ordered;

    size_t expectedCount;

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
               << " }";
    }
};

class Count
    : public ContainerBase
    , public testing::WithParamInterface<CountParam>
{
};


TEST_P(Count, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto count = c.count(TestValue(param.value));

    EXPECT_EQ(count, param.expectedCount);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Count, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto count = c.count(MiscInt(param.value));

    EXPECT_EQ(count, param.expectedCount);
    EXPECT_EQ(c.get_mode(), mode);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, Count,
    testing::Values(
        CountParam{"count_0",  0,  false, /*=*/  1 },
        CountParam{"count_2",  2,  false, /*=*/  2 },
        CountParam{"count_3",  3,  false, /*=*/  0 },
        CountParam{"count_6",  6,  false, /*=*/  1 },
        CountParam{"count_8",  8,  false, /*=*/  1 },
        CountParam{"count_10", 10, false, /*=*/  0 }),
    paramSetName<CountParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, Count,
    testing::Values(
        CountParam{"count_0",  0,  true, /*=*/  1 },
        CountParam{"count_2",  2,  true, /*=*/  1 },
        CountParam{"count_3",  3,  true, /*=*/  0 },
        CountParam{"count_6",  6,  true, /*=*/  1 },
        CountParam{"count_8",  8,  true, /*=*/  1 },
        CountParam{"count_10", 10, true, /*=*/  0 }),
    paramSetName<CountParam>);


// Find methods:
// find(const Key& key)
// find(const Key& key) const
// template<class K> find(const K& x)
// template<class K> find(const K& x) const
////////////////////////////////////////////////////////////////////////

struct FindParam {
    std::string name;

    int value;
    bool ordered;

    int expectedOffset;

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
               << " }";
    }
};


class Find
    : public ContainerBase
    , public testing::WithParamInterface<FindParam>
{
};


TEST_P(Find, NotConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    auto &c = ModeSwitch::container;

    auto iter = c.find(TestValue(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, Const) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto iter = c.find(TestValue(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, MiscNotConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    auto &c = ModeSwitch::container;

    auto iter = c.find(MiscInt(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Find, MiscConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto iter = c.find(MiscInt(param.value));
    auto offset = (iter == c.end() ? -1 : std::distance(c.begin(), iter));

    EXPECT_EQ(offset, param.expectedOffset);
    EXPECT_EQ(c.get_mode(), mode);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, Find,
    testing::Values(
        FindParam{"find_0",  0,  false, /*=*/  1 },
        FindParam{"find_2",  2,  false, /*=*/  2 },
        FindParam{"find_3",  3,  false, /*=*/ -1 },
        FindParam{"find_6",  6,  false, /*=*/  5 },
        FindParam{"find_8",  8,  false, /*=*/  0 },
        FindParam{"find_10", 10, false, /*=*/ -1 }),
    paramSetName<FindParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, Find,
    testing::Values(
        FindParam{"find_0",  0,  true, /*=*/  0 },
        FindParam{"find_2",  2,  true, /*=*/  1 },
        FindParam{"find_3",  3,  true, /*=*/ -1 },
        FindParam{"find_6",  6,  true, /*=*/  3 },
        FindParam{"find_8",  8,  true, /*=*/  4 },
        FindParam{"find_10", 10, true, /*=*/ -1 }),
    paramSetName<FindParam>);


// Containe methods:
// contains(const Key& key) const
// template<class K> contains(const K& x) const
////////////////////////////////////////////////////////////////////////


struct ContainsParam {
    std::string name;

    int value;
    bool ordered;

    bool expectedContains;

    friend std::ostream&
    operator<<(std::ostream& os, const ContainsParam& param) {
        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " value=" << param.value
               << ", ordered=" << param.ordered
               << " || expected:"
               << " contains=" << param.expectedContains
               << " }";
    }
};

class Contains
    : public ContainerBase
    , public testing::WithParamInterface<ContainsParam>
{
};


TEST_P(Contains, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto contains = c.contains(TestValue(param.value));

    EXPECT_EQ(contains, param.expectedContains);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(Contains, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto contains = c.contains(MiscInt(param.value));

    EXPECT_EQ(contains, param.expectedContains);
    EXPECT_EQ(c.get_mode(), mode);
}

INSTANTIATE_TEST_SUITE_P(
    unordered, Contains,
    testing::Values(
        ContainsParam{"contains_0",  0,  false, /*=*/  true },
        ContainsParam{"contains_2",  2,  false, /*=*/  true },
        ContainsParam{"contains_3",  3,  false, /*=*/  false },
        ContainsParam{"contains_6",  6,  false, /*=*/  true },
        ContainsParam{"contains_8",  8,  false, /*=*/  true },
        ContainsParam{"contains_10", 10, false, /*=*/  false }),
    paramSetName<ContainsParam>);

INSTANTIATE_TEST_SUITE_P(
    ordered, Contains,
    testing::Values(
        ContainsParam{"contains_0",  0,  true, /*=*/  true },
        ContainsParam{"contains_2",  2,  true, /*=*/  true },
        ContainsParam{"contains_3",  3,  true, /*=*/  false },
        ContainsParam{"contains_6",  6,  true, /*=*/  true },
        ContainsParam{"contains_8",  8,  true, /*=*/  true },
        ContainsParam{"contains_10", 10, true, /*=*/  false }),
    paramSetName<ContainsParam>);

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


struct RangeBoundParam {
    enum class func : char { equal_range, lower_bound, upper_bound };

    std::string name;

    int value;
    bool ordered;
    func function;

    int expectedLowerBoundOffset;
    int expectedUpperBoundOffset;

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
               << " }";
    }
};

class RangeBound
    : public ContainerBase
    , public testing::WithParamInterface<RangeBoundParam>
{
};


TEST_P(RangeBound, NotConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    auto &c = ModeSwitch::container;

    auto offsetOfIter =
        [](ContainerBase::Container::iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(TestValue(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(RangeBound, Const) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto offsetOfIter =
        [](ContainerBase::Container::const_iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(TestValue(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}

#if __cplusplus >= 201402L

TEST_P(RangeBound, MiscNotConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    auto &c = ModeSwitch::container;

    auto offsetOfIter =
        [](ContainerBase::Container::iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(MiscInt(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}


TEST_P(RangeBound, MiscConst) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    ModeSwitch::container.set_mode(mode);

    const auto &c = ModeSwitch::container;

    auto offsetOfIter =
        [](ContainerBase::Container::const_iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(MiscInt(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}
#endif // __cplusplus >= 201402L


INSTANTIATE_TEST_SUITE_P(
    unorderedEqualRange, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedEqualRange, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::equal_range,
            /*=*/  0, 1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::equal_range,
            /*=*/  1, 2 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::equal_range,
            /*=*/  2, 2 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::equal_range,
            /*=*/  3, 4 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::equal_range,
            /*=*/  4, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::equal_range,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedLowerBound, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedLowerBound, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::lower_bound,
            /*=*/  0, -1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::lower_bound,
            /*=*/  1, -1 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::lower_bound,
            /*=*/  2, -1 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::lower_bound,
            /*=*/  3, -1 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::lower_bound,
            /*=*/  4, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::lower_bound,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedUpperBound, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedUpperBound, RangeBound,
    testing::Values(
        RangeBoundParam{
            "range_bound_0", 0, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, 1 },
        RangeBoundParam{
            "range_bound_2", 2, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, 2 },
        RangeBoundParam{
            "range_bound_3", 3, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, 2 },
        RangeBoundParam{
            "range_bound_6", 6, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, 4 },
        RangeBoundParam{
            "range_bound_8", 8, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 },
        RangeBoundParam{
            "range_bound_10", 10, false, RangeBoundParam::func::upper_bound,
            /*=*/  -1, -1 }),
    paramSetName<RangeBoundParam>);


// Comparators
////////////////////////////////////////////////////////////////////////
class Comparators : public testing::Test {
};


/**
 * Compare class operator for class TestValue
 */
struct LessTestValue {
    /// Flag: LessTestValue::operator() has been used.
    static bool lessTestValueUsed;

    bool
    operator()(const TestValue &lhv, const TestValue &rhv) const {
        lessTestValueUsed = true;
        return (lhv.val < rhv.val);
    }
};

bool LessTestValue::lessTestValueUsed = false;


template<class T>
static void
comparatorLookupWork(T *c, bool ordered) {
    c->set_mode(
        ordered
        ? vectorset_mode::unique_ordered
        : vectorset_mode::unordered);

    const auto constC = c;

    auto findIter = c->find(TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), findIter), 2);

    auto cFindIter = constC->find(TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cFindIter), 2);

    if (!ordered) {
        return;
    }

    auto equalRange = c->equal_range(TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), equalRange.first), 2);
    EXPECT_EQ(std::distance(c->begin(), equalRange.second), 3);

    auto cEqualRange = constC->equal_range(TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cEqualRange.first), 2);
    EXPECT_EQ(std::distance(constC->begin(), cEqualRange.second), 3);

    auto lowerIter = c->lower_bound(TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), lowerIter), 2);

    auto cLowerIter = constC->lower_bound(TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cLowerIter), 2);

    auto upperIter = c->upper_bound(TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), upperIter), 3);

    auto cUpperIter = constC->upper_bound(TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), cUpperIter), 3);
}

template<class T>
static void
comparatorWork(T *c) {
    c->set_mode(vectorset_mode::unique_ordered);

    c->insert(TestValue(3));
    c->erase(3);

    c->insert(c->begin(), TestValue(3));
    c->erase(3);

    TestValue value(3);
    c->insert(value);
    c->erase(3);

    c->insert(c->begin(), value);
    c->erase(3);

    std::vector<TestValue> vals = { 1, 3 };
    c->insert(vals.begin(), vals.end());
    c->erase(1);
    c->erase(3);

    c->emplace(3);
    c->erase(3);

    c->emplace_hint(c->begin(), 3);
    c->erase(3);

    // Lookup unique ordered.
    comparatorLookupWork(c, true);
    comparatorLookupWork(c, false);
}


TEST_F(Comparators, Default) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    TestValue::lessUsed = false;

    vectorset<TestValue> c(values.begin(), values.end());

    comparatorWork(&c);
    EXPECT_EQ(c, result);

    EXPECT_TRUE(TestValue::lessUsed);
}


TEST_F(Comparators, ClassComp) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    TestValue::lessUsed = false;
    LessTestValue::lessTestValueUsed = false;

    vectorset<TestValue, LessTestValue> c(values.begin(), values.end());

    comparatorWork(&c);
    EXPECT_EQ(c, result);

    EXPECT_FALSE(TestValue::lessUsed);
    EXPECT_TRUE(LessTestValue::lessTestValueUsed);
}


TEST_F(Comparators, LambdaComp) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    TestValue::lessUsed = false;
    bool lessTestValueFnUsed = false;


    auto lessTestValueFn =
        [&lessTestValueFnUsed]
        (const TestValue &lhv, const TestValue &rhv) {
            lessTestValueFnUsed = true;
            return (lhv.val < rhv.val);
        };

    vectorset<TestValue, decltype(lessTestValueFn)> c(
        values.begin(), values.end(), lessTestValueFn);

    comparatorWork(&c);
    EXPECT_EQ(c, result);

    EXPECT_FALSE(TestValue::lessUsed);
    EXPECT_TRUE(lessTestValueFnUsed);
}

// Allocators
////////////////////////////////////////////////////////////////////////


class Allocators : public testing::Test {
};


/// Flag: allocate/deallocate has been used.
static bool testAllocUsed = false;


/**
 * Allocator class operator for class TestValue
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

    vectorset<TestValue, std::less<TestValue>, TestAlloc<TestValue>> c(
        values.begin(), values.end());

    EXPECT_EQ(c, values);
    EXPECT_TRUE(testAllocUsed);
}


// Boolean Compare Operators
////////////////////////////////////////////////////////////////////////

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
               << ", greater or equal than=" << param.expectedGreaterOrEqualThan
               << " }";
    }
};

class CompBoolOp
    : public ContainerBase
    , public testing::WithParamInterface<CompBoolOpParam>
{
};


TEST_P(CompBoolOp, any) {
    const auto &param = GetParam();
    const auto &c = CompBoolOp::container;

    CompBoolOp::Container compVals;
    std::transform(
        param.values.begin(), param.values.end(),
        inserter(compVals, compVals.end()),
        [](const int value) {
            return TestValue(value);
        });

    EXPECT_EQ(compVals == c, param.expectedEqual);
    EXPECT_EQ(compVals != c, param.expectedNotEqual);
    EXPECT_EQ(compVals < c,  param.expectedLessThan);
    EXPECT_EQ(compVals <= c, param.expectedLessOrEqualThan);
    EXPECT_EQ(compVals > c,  param.expectedGreaterThan);
    EXPECT_EQ(compVals >= c, param.expectedGreaterOrEqualThan);
}


INSTANTIATE_TEST_SUITE_P(
    all, CompBoolOp,
    testing::Values(
        CompBoolOpParam{
            "equal", { 8, 0, 2, 5, 2, 6 },
            /* = */ true, false, /**/ false, true, /**/ false, true },
        CompBoolOpParam{
            "shorter", { 8, 0, 2, 5, 2 },
            /* = */ false, true, /**/ true, true, /**/ false, false },
        CompBoolOpParam{
            "longer", { 8, 0, 2, 5, 2, 6, 8 },
            /* = */ false, true, /**/ false, false, /**/ true, true },
        CompBoolOpParam{
            "lessEqLen", { 8, 0, 2, 4, 2, 6 },
            /* = */ false, true, /**/ true, true, /**/ false, false },
        CompBoolOpParam{
            "greaterEqLen", { 8, 0, 2, 6, 2, 6 },
            /* = */ false, true, /**/ false, false, /**/ true, true },
        CompBoolOpParam{
            "lessShorter", { 8, 0, 2, 4, 2},
            /* = */ false, true, /**/ true, true, /**/ false, false },
        CompBoolOpParam{
            "greaterShorter", { 8, 0, 2, 6, 2},
            /* = */ false, true, /**/ false, false, /**/ true, true },
        CompBoolOpParam{
            "lessLonger", { 8, 0, 2, 4, 2, 6, 8},
            /* = */ false, true, /**/ true, true, /**/ false, false },
        CompBoolOpParam{
            "greaterLonger", { 8, 0, 2, 6, 2, 6, 8 },
            /* = */ false, true, /**/ false, false, /**/ true, true }
        ),
    paramSetName<CompBoolOpParam>);


#if __cplusplus >= 202002L // c++20

// Three Way Compare Operator
////////////////////////////////////////////////////////////////////////


struct CompThreeWayOpParam {
    std::string name;

    std::vector<int> values;

    std::strong_ordering expectedResult;

    friend std::ostream&
    operator<<(std::ostream& os, const CompThreeWayOpParam& param) {
        const char *resultStr = nullptr;
        if (param.expectedResult == std::strong_ordering::equal) {
            resultStr = "equal";
        }
        else if (param.expectedResult == std::strong_ordering::less) {
            resultStr = "less";
        }
        else if (param.expectedResult == std::strong_ordering::greater) {
            resultStr = "greater";
        }
        else {
            resultStr = "*unexpected*";
        }

        return
            os << std::boolalpha
               << "{ name=\"" << param.name << "\""
               << " || input:"
               << " values=" << param.values
               << " || expected:"
               << " result=" << resultStr
               << " }";
    }
};

class CompThreeWayOp
    : public ContainerBase
    , public testing::WithParamInterface<CompThreeWayOpParam>
{
};


TEST_P(CompThreeWayOp, any) {
    const auto &param = GetParam();
    const auto &c = CompThreeWayOp::container;

    CompThreeWayOp::Container compVals;
    std::transform(
        param.values.begin(), param.values.end(),
        inserter(compVals, compVals.end()),
        [](const int value) {
            return TestValue(value);
        });

    EXPECT_EQ(compVals <=> c, param.expectedResult);
}

INSTANTIATE_TEST_SUITE_P(
    all, CompThreeWayOp,
    testing::Values(
        CompThreeWayOpParam{
            "equal", { 8, 0, 2, 5, 2, 6 },
            /* = */ std::strong_ordering::equal},
        CompThreeWayOpParam{
            "shorter", { 8, 0, 2, 5, 2 },
            /* = */ std::strong_ordering::less },
        CompThreeWayOpParam{
            "longer", { 8, 0, 2, 5, 2, 6, 8 },
            /* = */ std::strong_ordering::greater },
        CompThreeWayOpParam{
            "lessEqLen", { 8, 0, 2, 4, 2, 6 },
             /* = */ std::strong_ordering::less },
        CompThreeWayOpParam{
            "greaterEqLen", { 8, 0, 2, 6, 2, 6 },
            /* = */ std::strong_ordering::greater },
        CompThreeWayOpParam{
            "lessShorter", { 8, 0, 2, 4, 2},
             /* = */ std::strong_ordering::less },
        CompThreeWayOpParam{
            "greaterShorter", { 8, 0, 2, 6, 2},
            /* = */ std::strong_ordering::greater },
        CompThreeWayOpParam{
            "lessLonger", { 8, 0, 2, 4, 2, 6, 8},
             /* = */ std::strong_ordering::less },
        CompThreeWayOpParam{
            "greaterLonger", { 8, 0, 2, 6, 2, 6, 8 },
            /* = */ std::strong_ordering::greater }
        ),
    paramSetName<CompThreeWayOpParam>);


#endif // c++20


// Main Test Program
////////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
