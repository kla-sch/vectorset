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

#include "Comparators.h"

#include "TestUtils.h"

/**
 * Compare class operator for class vs_test::TestValue
 */
struct LessTestValue {
    /// Flag: LessTestValue::operator() has been used.
    static bool lessTestValueUsed;

    bool
    operator()(
        const vs_test::TestValue &lhv,
        const vs_test::TestValue &rhv) const
    {
        lessTestValueUsed = true;
        return (lhv.val < rhv.val);
    }
};

bool LessTestValue::lessTestValueUsed = false;


/*
 * Test workload: lookup functions.
 */
template<class T>
static void
comparatorLookupWork(T *c, bool ordered) {
    c->set_mode(
        ordered
        ? vectorset_mode::unique_ordered
        : vectorset_mode::unordered);

    const auto *const constC = c;

    auto findIter = c->find(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), findIter), 2);

    auto cFindIter = constC->find(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cFindIter), 2);

    if (!ordered) {
        return;
    }

    auto equalRange = c->equal_range(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), equalRange.first), 2);
    EXPECT_EQ(std::distance(c->begin(), equalRange.second), 3);

    auto cEqualRange = constC->equal_range(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cEqualRange.first), 2);
    EXPECT_EQ(std::distance(constC->begin(), cEqualRange.second), 3);

    auto lowerIter = c->lower_bound(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), lowerIter), 2);

    auto cLowerIter = constC->lower_bound(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cLowerIter), 2);

    auto upperIter = c->upper_bound(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(c->begin(), upperIter), 3);

    auto cUpperIter = constC->upper_bound(vs_test::TestValue(5));
    EXPECT_EQ(std::distance(constC->begin(), cUpperIter), 3);
}

/*
 * Test workload to trigger compare operations.
 */
template<class T>
static void
comparatorWork(T *c) {
    c->set_mode(vectorset_mode::unique_ordered);

    c->insert(vs_test::TestValue(3));
    c->erase(3);

    c->insert(c->begin(), vs_test::TestValue(3));
    c->erase(3);

    vs_test::TestValue value(3);
    c->insert(value);
    c->erase(3);

    c->insert(c->begin(), value);
    c->erase(3);

    std::vector<vs_test::TestValue> vals = { 1, 3 };
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

// Supposed trace
const _Vectorset_Trace Comparators::workTrace = {
    {"vectorset(InputIt,InputIt,const Compare&,const Allocator&)", "construct"},

    {"set_mode(vectorset_mode)", "start"},
    {"make_unique_ordered()", "start"},
    {"make_unique_ordered()", "rmIter:5(6)"},
    {"set_mode(vectorset_mode)", "mode-ordered"},

    {"impl_insert_unique_ordered(ValType&&)", "start"},
    {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
    {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "start:0(5)"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "not-empty"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)",
     "call:impl_insert_unique_ordered(ValType&&)"},
    {"impl_insert_unique_ordered(ValType&&)", "start"},
    {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
    {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "insert:true"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "result:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_insert_unique_ordered(ValType&&)", "start"},
    {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
    {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "start:0(5)"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "not-empty"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)",
     "call:impl_insert_unique_ordered(ValType&&)"},
    {"impl_insert_unique_ordered(ValType&&)", "start"},
    {"impl_insert_unique_ordered(ValType&&)", "lower_bound:2(5)"},
    {"impl_insert_unique_ordered(ValType&&)", "insert:2(6)"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "insert:true"},
    {"impl_insert_unique_ordered(const_iterator,ValType&&)", "result:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"insert(InputIt,InputIt)", "start"},
    {"insert(InputIt,InputIt)", "call:make_unique_ordered()"},
    {"make_unique_ordered()", "start"},
    {"make_unique_ordered()", "rmIter:7(7)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:1(7)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_emplace_unique_ordered(Args&&...)", "start"},
    {"impl_insert_emplace_unique_ordered", "start"},
    {"impl_insert_emplace_unique_ordered", "last:5(6)"},
    {"impl_insert_emplace_unique_ordered", "iter:2(6)"},
    {"impl_insert_emplace_unique_ordered", "move-into-position"},
    {"move_into_position", "start:5(6) to 2(6)"},
    {"move_into_position", "swap:4(6) and 5(6)"},
    {"move_into_position", "swap:3(6) and 4(6)"},
    {"move_into_position", "swap:2(6) and 3(6)"},
    {"impl_emplace_unique_ordered(Args&&...)", "insert:true"},
    {"impl_emplace_unique_ordered(Args&&...)", "iter:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "start:0(5)"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "hintDistance:0"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "valIter:5(6)"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "hintIter:0(6)"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "not-empty"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "normal-emplace"},
    {"impl_insert_emplace_unique_ordered", "start"},
    {"impl_insert_emplace_unique_ordered", "last:5(6)"},
    {"impl_insert_emplace_unique_ordered", "iter:2(6)"},
    {"impl_insert_emplace_unique_ordered", "move-into-position"},
    {"move_into_position", "start:5(6) to 2(6)"},
    {"move_into_position", "swap:4(6) and 5(6)"},
    {"move_into_position", "swap:3(6) and 4(6)"},
    {"move_into_position", "swap:2(6) and 3(6)"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "emplaced:true"},
    {"impl_emplace_unique_ordered(const_iterator,Args&&...)", "result:2(6)"},

    {"impl_erase_unique_ordered(const Key&)", "start"},
    {"impl_erase_unique_ordered(const Key&)", "iter:2(6)"},
    {"impl_erase_unique_ordered(const Key&)", "found-erase:1"},

    // comparatorLookupWork(c, true)
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"find(const Key&)", "start"},
    {"impl_find_unique_ordered(const_reference)", "start"},
    {"impl_find_unique_ordered(const_reference)", "iter:2(5)"},

    {"find(const Key&)const", "start"},
    {"impl_find_unique_ordered(const_reference)const", "start"},
    {"impl_find_unique_ordered(const_reference)const", "iter:2(5)"},

    {"equal_range(const Key&)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "iter:2(5)"},
    {"impl_equal_range(const Value&, AlgoComp)", "range:2(5) to 3(5)"},

    {"equal_range(const Key&)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "iter:2(5)"},
    {"impl_equal_range(const Value&, AlgoComp)const", "range:2(5) to 3(5)"},

    {"lower_bound(const Key&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"},

    {"lower_bound(const Key&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"},

    {"upper_bound(const Key&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:3(5)"},

    {"upper_bound(const Key&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:3(5)"},

    // comparatorLookupWork(c, false)
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-unordered"},

    {"find(const Key&)", "start"},
    {"impl_find_unordered(const_reference)", "start"},
    {"impl_find_unordered(const_reference)", "iter:2(5)"},

    {"find(const Key&)const", "start"},
    {"impl_find_unordered(const_reference)const", "start"},
    {"impl_find_unordered(const_reference)const", "iter:2(5)"}
};

TEST_F(Comparators, Default) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    vs_test::TestValue::lessUsed = false;

    vectorset<vs_test::TestValue> c(values.begin(), values.end());

    comparatorWork(&c);
    EXPECT_EQ(c, result);
    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), workTrace));

    EXPECT_TRUE(vs_test::TestValue::lessUsed);
}


TEST_F(Comparators, ClassComp) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    vs_test::TestValue::lessUsed = false;
    LessTestValue::lessTestValueUsed = false;

    vectorset<vs_test::TestValue, LessTestValue>
        c(values.begin(), values.end());

    comparatorWork(&c);
    EXPECT_EQ(c, result);
    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), workTrace));

    EXPECT_FALSE(vs_test::TestValue::lessUsed);
    EXPECT_TRUE(LessTestValue::lessTestValueUsed);
}


TEST_F(Comparators, LambdaComp) {
    std::vector<int> values = { 8, 0, 2, 5, 2, 6 };
    std::vector<int> result = { 0, 2, 5, 6, 8 };

    vs_test::TestValue::lessUsed = false;
    bool lessTestValueFnUsed = false;


    auto lessTestValueFn =
        [&lessTestValueFnUsed]
        (const vs_test::TestValue &lhv, const vs_test::TestValue &rhv) {
            lessTestValueFnUsed = true;
            return (lhv.val < rhv.val);
        };

    vectorset<vs_test::TestValue, decltype(lessTestValueFn)> c(
        values.begin(), values.end(), lessTestValueFn);

    comparatorWork(&c);
    EXPECT_EQ(c, result);
    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), workTrace));

    EXPECT_FALSE(vs_test::TestValue::lessUsed);
    EXPECT_TRUE(lessTestValueFnUsed);
}
