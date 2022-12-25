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
#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include <string>
#include <vector>
#include <iostream>

#include "gtest/gtest.h"

#include "TestUtils.h"

// Test cases
#include "Constructor.h"
#include "ModeSwitch.h"
#include "ConstructorCopyMove.h"
#include "Insert.h"
#include "InsertPos.h"
#include "InsertByIter.h"
#include "InsertByInitList.h"
#include "Emplace.h"
#include "EmplaceHint.h"
#include "EraseAtPos.h"
#include "EraseRange.h"
#include "EraseByKey.h"
#include "MiscManipulationSwap.h"
#include "Count.h"
#include "Find.h"
#include "Contains.h"
#include "RangeBound.h"
#include "Comparators.h"
#include "Allocators.h"
#include "CompBoolOp.h"
#include "CompThreeWayOp.h"
#include "Debug.h"

// Basic constructors:
// vectorset()
// vectorset(std::initializer_list<value_type> init)
// vectorset(InputIt first, InputIt last)
////////////////////////////////////////////////////////////////////////

// see Constructor.cpp


// Mode switch:
// set_mode(vectorset_mode)
////////////////////////////////////////////////////////////////////////

// see ModeSwitch.cpp


// Copy & Move constructor:
// vectorset(const vectorset& other)
// vectorset(vectorset&& other)
////////////////////////////////////////////////////////////////////////

// see ConstructorCopyMove.cpp


// Plain insert methods:
// insert(const value_type &)
// insert(value_type &&)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, Insert,
    testing::ValuesIn(vs_test::InsertParam::caseUnordered),
    vs_test::paramSetName<vs_test::InsertParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, Insert,
    testing::ValuesIn(vs_test::InsertParam::caseOrdered),
    vs_test::paramSetName<vs_test::InsertParam>);


// insert at position methods:
// insert(const_iterator pos, const value_type &)
// insert(const_iterator pos, value_type &&)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unorderedPosBegin, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseUnorderedPosBegin),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPos2, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseUnorderedPos2),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosEnd, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseUnorderedPosEnd),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosMatch, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseUnorderedPosMatch),
    vs_test::paramSetName<vs_test::InsertPosParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    orderedPosBegin, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPosBegin),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPos2, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPos2),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosEnd, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPosEnd),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchMiddle, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPosMatchMiddle),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchBegin, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPosMatchBegin),
    vs_test::paramSetName<vs_test::InsertPosParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchEnd, InsertPos,
    testing::ValuesIn(vs_test::InsertPosParam::caseOrderedPosMatchEnd),
    vs_test::paramSetName<vs_test::InsertPosParam>);


// insert by iterators:
// insert(InputIter first, InputIter last)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, InsertByIter,
    testing::ValuesIn(vs_test::InsertByIterParam::caseUnordered),
    vs_test::paramSetName<vs_test::InsertByIterParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, InsertByIter,
    testing::ValuesIn(vs_test::InsertByIterParam::caseOrdered),
    vs_test::paramSetName<vs_test::InsertByIterParam>);


// insert by initializer_list:
// insert(std::initializer_list<value_type> ilist)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, InsertByInitList,
    testing::ValuesIn(vs_test::InsertByInitListParam::caseUnordered),
    vs_test::paramSetName<vs_test::InsertByInitListParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, InsertByInitList,
    testing::ValuesIn(vs_test::InsertByInitListParam::caseOrdered),
    vs_test::paramSetName<vs_test::InsertByInitListParam>);


// Plain emplace method:
// emplace(Args&&... args)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, Emplace,
    testing::ValuesIn(vs_test::EmplaceParam::caseUnordered),
    vs_test::paramSetName<vs_test::EmplaceParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, Emplace,
    testing::ValuesIn(vs_test::EmplaceParam::caseOrdered),
    vs_test::paramSetName<vs_test::EmplaceParam>);


// Emplace with hint method:
// emplace_hint(const_iterator hint, Args&&... args)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unorderedPosBegin, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseUnorderedPosBegin),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPos2, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseUnorderedPos2),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosEnd, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseUnorderedPosEnd),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    unorderedPosMatch, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseUnorderedPosMatch),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    orderedPosBegin, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPosBegin),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPos2, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPos2),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosEnd, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPosEnd),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchMiddle, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPosMatchMiddle),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchBegin, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPosMatchBegin),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedPosMatchEnd, EmplaceHint,
    testing::ValuesIn(vs_test::EmplaceHintParam::caseOrderedPosMatchEnd),
    vs_test::paramSetName<vs_test::EmplaceHintParam>);


// erase at position method:
// erase(const_iterator pos)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, EraseAtPos,
    testing::ValuesIn(vs_test::EraseAtPosParam::caseUnordered),
    vs_test::paramSetName<vs_test::EraseAtPosParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, EraseAtPos,
    testing::ValuesIn(vs_test::EraseAtPosParam::caseOrdered),
    vs_test::paramSetName<vs_test::EraseAtPosParam>);


// erase range method:
// erase(const_iterator first, const_iterator last)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, EraseRange,
    testing::ValuesIn(vs_test::EraseRangeParam::caseUnordered),
    vs_test::paramSetName<vs_test::EraseRangeParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, EraseRange,
    testing::ValuesIn(vs_test::EraseRangeParam::caseOrdered),
    vs_test::paramSetName<vs_test::EraseRangeParam>);


// erase by key method:
// erase(const Key& key)
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, EraseByKey,
    testing::ValuesIn(vs_test::EraseByKeyParam::caseUnordered),
    vs_test::paramSetName<vs_test::EraseByKeyParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, EraseByKey,
    testing::ValuesIn(vs_test::EraseByKeyParam::caseOrdered),
    vs_test::paramSetName<vs_test::EraseByKeyParam>);


// vector manipulation:
// push_back(const Key& value)
// push_back(Key&& value)
// pop_back()
// resize(size_type count)
// resize(size_type count, const value_type& value)
////////////////////////////////////////////////////////////////////////

// see VectorManipulation.cpp


// miscellaneous manipulation:
// swap(vectorset& other)
////////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    any, MiscManipulationSwap,
    testing::ValuesIn(vs_test::MiscManipulationSwapParam::caseAny),
    vs_test::paramSetName<vs_test::MiscManipulationSwapParam>);


// Count methods:
// count(const Key& key) const
// template<class K> count(const K& x) const
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, Count,
    testing::ValuesIn(vs_test::CountParam::caseUnordered),
    vs_test::paramSetName<vs_test::CountParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, Count,
    testing::ValuesIn(vs_test::CountParam::caseOrdered),
    vs_test::paramSetName<vs_test::CountParam>);


// Find methods:
// find(const Key& key)
// find(const Key& key) const
// template<class K> find(const K& x)
// template<class K> find(const K& x) const
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, Find,
    testing::ValuesIn(vs_test::FindParam::caseUnordered),
    vs_test::paramSetName<vs_test::FindParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, Find,
    testing::ValuesIn(vs_test::FindParam::caseOrdered),
    vs_test::paramSetName<vs_test::FindParam>);


// Contains methods:
// contains(const Key& key) const
// template<class K> contains(const K& x) const
////////////////////////////////////////////////////////////////////////

// unordered
INSTANTIATE_TEST_SUITE_P(
    unordered, Contains,
    testing::ValuesIn(vs_test::ContainsParam::caseUnordered),
    vs_test::paramSetName<vs_test::ContainsParam>);

// ordered
INSTANTIATE_TEST_SUITE_P(
    ordered, Contains,
    testing::ValuesIn(vs_test::ContainsParam::caseOrdered),
    vs_test::paramSetName<vs_test::ContainsParam>);


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

// equal_range
INSTANTIATE_TEST_SUITE_P(
    unorderedEqualRange, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseUnorderedEqualRange),
    vs_test::paramSetName<vs_test::RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedEqualRange, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseOrderedEqualRange),
    vs_test::paramSetName<vs_test::RangeBoundParam>);

// lower_bound
INSTANTIATE_TEST_SUITE_P(
    unorderedLowerBound, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseUnorderedLowerBound),
    vs_test::paramSetName<vs_test::RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedLowerBound, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseOrderedLowerBound),
    vs_test::paramSetName<vs_test::RangeBoundParam>);

// upper_bound
INSTANTIATE_TEST_SUITE_P(
    unorderedUpperBound, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseUnorderedUpperBound),
    vs_test::paramSetName<vs_test::RangeBoundParam>);

INSTANTIATE_TEST_SUITE_P(
    orderedUpperBound, RangeBound,
    testing::ValuesIn(vs_test::RangeBoundParam::caseOrderedUpperBound),
    vs_test::paramSetName<vs_test::RangeBoundParam>);


// Comparators
////////////////////////////////////////////////////////////////////////

// see Comparators.cpp


// Allocators
////////////////////////////////////////////////////////////////////////

// see Allocators.cpp


// Boolean Compare Operators
////////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    all, CompBoolOp,
    testing::ValuesIn(vs_test::CompBoolOpParam::caseAll),
    vs_test::paramSetName<vs_test::CompBoolOpParam>);


#if __cplusplus >= 202002L // c++20

// Three Way Compare Operator
////////////////////////////////////////////////////////////////////////

INSTANTIATE_TEST_SUITE_P(
    all, CompThreeWayOp,
    testing::ValuesIn(vs_test::CompThreeWayOpParam::caseAll),
    vs_test::paramSetName<vs_test::CompThreeWayOpParam>);

#endif // c++20


// Main Test Program
////////////////////////////////////////////////////////////////////////

int
main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
