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

#include "RangeBound.h"


TEST_P(RangeBound, Key) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    auto &c = container;

    auto offsetOfIter =
        [&c](vs_test::ContainerBase::Container::iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case vs_test::RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(vs_test::TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(vs_test::TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(vs_test::TestValue(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceKey));
    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}

TEST_P(RangeBound, ConstKey) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto offsetOfIter =
        [&c](vs_test::ContainerBase::Container::const_iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case vs_test::RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(vs_test::TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(vs_test::TestValue(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(vs_test::TestValue(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceConstKey));
    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}

#if RANGE_BOUND_WITH_MISC_VALUE

TEST_P(RangeBound, Misc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    auto &c = container;

    auto offsetOfIter =
        [&c](vs_test::ContainerBase::Container::iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case vs_test::RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(vs_test::MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(vs_test::MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(vs_test::MiscInt(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceMisc));
    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}


TEST_P(RangeBound, ConstMisc) {
    const auto &param = GetParam();

    auto mode =
        (param.ordered
         ? vectorset_mode::unique_ordered
         : vectorset_mode::unordered);

    container.set_mode(mode);

    const auto &c = container;

    auto offsetOfIter =
        [&c](vs_test::ContainerBase::Container::const_iterator iter) {
            return (iter == c.end() ? -1 : std::distance(c.begin(), iter));
        };

    bool gotLogicError = false;
    try {
        switch (param.function) {
        case vs_test::RangeBoundParam::func::equal_range: {
            auto iterPair = c.equal_range(vs_test::MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iterPair.first),
                      param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iterPair.second),
                      param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::lower_bound: {
            auto iter = c.lower_bound(vs_test::MiscInt(param.value));
            EXPECT_EQ(offsetOfIter(iter), param.expectedLowerBoundOffset);
            EXPECT_EQ(-1, param.expectedUpperBoundOffset);
            break;
        }

        case vs_test::RangeBoundParam::func::upper_bound: {
            auto iter = c.upper_bound(vs_test::MiscInt(param.value));
            EXPECT_EQ(-1, param.expectedLowerBoundOffset);
            EXPECT_EQ(offsetOfIter(iter), param.expectedUpperBoundOffset);
            break;
        }

        } // switch (param.function)
    }
    catch (const std::logic_error &) {
        gotLogicError = true;
    }

    EXPECT_TRUE(vs_test::checkTrace(c.getDebugTrace(), param.traceConstMisc));
    EXPECT_EQ(gotLogicError, !param.ordered);
    EXPECT_EQ(c.get_mode(), mode);
}
#endif // __cplusplus >= 201402L

static const _Vectorset_Trace equalRangeLogicErrorKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"equal_range(const Key&)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace equalRangeLogicErrorConstKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"equal_range(const Key&)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "logic_error"}
};


#if RANGE_BOUND_WITH_MISC_VALUE
static const _Vectorset_Trace equalRangeLogicErrorMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"equal_range(const K&)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "start"},
    {"impl_equal_range(const Value&, AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace equalRangeLogicErrorConstMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"equal_range(const K&)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "start"},
    {"impl_equal_range(const Value&, AlgoComp)const", "logic_error"}
};
#endif

// Test parameters:
std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseUnorderedEqualRange = {
    {
        "range_bound_0", 0, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }, {
        "range_bound_2", 2, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }, {
        "range_bound_3", 3, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }, {
        "range_bound_6", 6, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }, {
        "range_bound_8", 8, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }, {
        "range_bound_10", 10, false,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        equalRangeLogicErrorKey, equalRangeLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , equalRangeLogicErrorMisc, equalRangeLogicErrorConstMisc
#endif
    }
};


std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseOrderedEqualRange({
    {
        "range_bound_0", 0, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  0, 1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:0(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:0(5) to 1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:0(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:0(5) to 1(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:0(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:0(5) to 1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:0(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:0(5) to 1(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_2", 2, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  1, 2,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:1(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:1(5) to 2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:1(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:1(5) to 2(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:1(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:1(5) to 2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:1(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:1(5) to 2(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_3", 3, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  2, 2,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:2(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "value<*iter"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:2(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "value<*iter"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:2(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "value<*iter"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:2(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "value<*iter"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_6", 6, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  3, 4,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:3(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:3(5) to 4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:3(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:3(5) to 4(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:3(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:3(5) to 4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:3(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:3(5) to 4(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_8", 8, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  4, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:4(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:4(5) to 5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:4(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:4(5) to 5(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:4(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "range:4(5) to 5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:4(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "range:4(5) to 5(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_10", 10, true,
        vs_test::RangeBoundParam::func::equal_range,
        /*=*/  -1, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:5(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter-at-end"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const Key&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:5(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter-at-end"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)", "start"},

            {"impl_equal_range(const Value&, AlgoComp)",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter:5(5)"},
            {"impl_equal_range(const Value&, AlgoComp)",
             "iter-at-end"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"equal_range(const K&)const", "start"},

            {"impl_equal_range(const Value&, AlgoComp)const",
             "start"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter:5(5)"},
            {"impl_equal_range(const Value&, AlgoComp)const",
             "iter-at-end"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }
});


static const _Vectorset_Trace lowerBoundLogicErrorKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"lower_bound(const Key&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace lowerBoundLogicErrorConstKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"lower_bound(const Key&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "logic_error"}
};


#if RANGE_BOUND_WITH_MISC_VALUE
static const _Vectorset_Trace lowerBoundLogicErrorMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"lower_bound(const K&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace lowerBoundLogicErrorConstMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"lower_bound(const K&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "logic_error"}
};
#endif



std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseUnorderedLowerBound = {
    {
        "range_bound_0", 0, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_2", 2, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_3", 3, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_6", 6, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_8", 8, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_10", 10, false,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        lowerBoundLogicErrorKey, lowerBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , lowerBoundLogicErrorMisc, lowerBoundLogicErrorConstMisc
#endif
    }
};


std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseOrderedLowerBound = {
    {
        "range_bound_0", 0, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  0, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:0(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:0(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:0(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_2", 2, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  1, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:1(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:1(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_3", 3, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  2, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_6", 6, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  3, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:3(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:3(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:3(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_8", 8, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  4, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:4(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:4(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
 +       "range_bound_10", 10, true,
        vs_test::RangeBoundParam::func::lower_bound,
        /*=*/  -1, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"lower_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }
};

static const _Vectorset_Trace upperBoundLogicErrorKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"upper_bound(const Key&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace upperBoundLogicErrorConstKey = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"upper_bound(const Key&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "logic_error"}
};


#if RANGE_BOUND_WITH_MISC_VALUE
static const _Vectorset_Trace upperBoundLogicErrorMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"upper_bound(const K&)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)", "logic_error"}
};

static const _Vectorset_Trace upperBoundLogicErrorConstMisc = {
    {"set_mode(vectorset_mode)", "start"},
    {"set_mode(vectorset_mode)", "mode-equal"},

    {"upper_bound(const K&)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
    {"impl_bound_fn(const Value&,bool,AlgoComp)const", "logic_error"}
};
#endif


std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseUnorderedUpperBound = {
    {
        "range_bound_0", 0, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_2", 2, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_3", 3, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_6", 6, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_8", 8, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }, {
        "range_bound_10", 10, false,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        upperBoundLogicErrorKey, upperBoundLogicErrorConstKey
#if RANGE_BOUND_WITH_MISC_VALUE
        , upperBoundLogicErrorMisc, upperBoundLogicErrorConstMisc
#endif
    }
};


std::vector<vs_test::RangeBoundParam>
vs_test::RangeBoundParam::caseOrderedUpperBound = {
    {
        "range_bound_0", 0, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, 1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:1(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:1(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:1(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_2", 2, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, 2,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_3", 3, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, 2,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:2(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:2(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_6", 6, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, 4,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:4(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:4(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:4(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_8", 8, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }, {
        "range_bound_10", 10, true,
        vs_test::RangeBoundParam::func::upper_bound,
        /*=*/  -1, -1,
        {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const Key&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#if RANGE_BOUND_WITH_MISC_VALUE
        , {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)", "iter:5(5)"}
        }, {
            {"set_mode(vectorset_mode)", "start"},
            {"make_unique_ordered()", "start"},
            {"make_unique_ordered()", "rmIter:5(6)"},
            {"set_mode(vectorset_mode)", "mode-ordered"},

            {"upper_bound(const K&)const", "start"},

            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "start"},
            {"impl_bound_fn(const Value&,bool,AlgoComp)const", "iter:5(5)"}
        }
#endif /* RANGE_BOUND_WITH_MISC_VALUE */
    }
};
