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

#ifndef VECTOR_SET_TEST_TEST_UTILS_HXX
#define VECTOR_SET_TEST_TEST_UTILS_HXX 1

#include <iostream>
#include <algorithm>

#define _VECTOR_SET_DO_TRACE 1
#include "vectorset.h"

#include "gtest/gtest.h"

namespace vs_test {
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

        TestValue(const TestValue &other) noexcept
            : val(other.val), copied(true)
        {
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

    /// Miscellaneous class for lookup tests.
    struct MiscInt {
        int val;

        MiscInt() = delete;
        MiscInt(int value) : val(value) {}
    };


    /**
     * Check, that none of container elements has been copied.
     */
    inline bool
    hasNoCopiedElem(const vectorset<vs_test::TestValue> &c) {
        return
            std::none_of(
                c.begin(), c.end(),
                [](const TestValue &item) {
                    return item.copied;
                });
    }

    /**
     * Compare two trace vectors.
     */
    inline testing::AssertionResult
    checkTrace(
        const _Vectorset_Trace &containerTrace,
        const _Vectorset_Trace &expectedTrace)
    {
        if (containerTrace == expectedTrace) {
            return testing::AssertionSuccess();
        }
        else {
            return (testing::AssertionFailure()
                    << "\n"
                    << ">>>>>>>>>>>>>>>>>>>> container trace\n"
                    << containerTrace << '\n'
                    << "<<<<<<<<<<<<<<<<<<<<\n\n"
                    << ">>>>>>>>>>>>>>>>>>>> expected trace\n"
                    << expectedTrace << '\n'
                    << "<<<<<<<<<<<<<<<<<<<<\n"
                );
        }
    }

    /// Get name of parameter set.
    template<class ParamStruct>
    inline std::string
    paramSetName(const testing::TestParamInfo<ParamStruct> &info) {
        return info.param.name;
    };


#if __cplusplus >= 201703L // compiler >= C++17

    /**
     * Creates vectorset of type T.
     *
     * @param args earch argument is an value of type T.
     */
    template <typename T, typename... Args>
    inline vectorset<T>
    make_vectorset(Args&&... args)
    {
        vectorset<T> result;
        result.reserve(sizeof...(Args));
        (result.insert(std::forward<Args>(args)), ...);
        return result;
    }

#else // compiler < C++17

    // generation helper for make_vectorset
    template<typename T>
    inline void
    make_vectorset_gen(vectorset<T> *result, T&& val) {
        result->push_back(std::forward<T>(val));
    }

    // generation helper for make_vectorset
    template<typename T, typename ...Next>
    inline void
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
    inline vectorset<T>
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
    inline std::vector<T>
    make_vector(Args&&... args)
    {
        std::vector<T> result;
        result.reserve(sizeof...(Args));
        (result.push_back(std::forward<Args>(args)), ...);
        return result;
    }

#else // __cplusplus < 201703L

    // generation helper for make_vector
    template<typename T>
    inline void
    make_vector_gen(std::vector<T> *result, T&& val) {
        result->push_back(std::forward<T>(val));
    }

    // generation helper for make_vector
    template<typename T, typename... Next>
    inline void
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
    inline std::vector<T>
    make_vector(Args&& ...args) {
        std::vector<T> result;
        result.reserve(sizeof...(args));
        make_vector_gen<T>(&result, std::forward<Args>(args)...);

        return result;
    }

#endif // __cplusplus < 201703L

    /// TestValue < MiscInt
    inline bool
    operator<(
        const vs_test::TestValue &cpt,
        const vs_test::MiscInt &mi) noexcept
    {
        return (cpt.val < mi.val);
    }

    /// MiscInt < TestValue
    inline bool
    operator<(
        const vs_test::MiscInt &mi,
        const vs_test::TestValue &cpt) noexcept
    {
        return (mi.val < cpt.val);
    }


} /* namespace vs_test */


// Operators in global namespace
// //////////////////////////////

/**
 * Write values of container with TestValue instances.
 */
inline std::ostream&
operator<<(std::ostream& os, const vectorset<vs_test::TestValue> &c) {
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

/// Print-Method for google test
inline void
PrintTo(const vectorset<vs_test::TestValue> &c, std::ostream* os) {
    *os << c;
}


/**
 *  Write values of vector with int.
 */
inline std::ostream&
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

/**
 * Compare result container.
 *
 * @param c Result container.
 * @param res expected result as vector.
 *
 * @return true, if the container contains the exprected result.
 */
template<class Compare, class Allocator>
inline bool
operator==(
    const vectorset<vs_test::TestValue, Compare, Allocator> &c,
    const std::vector<int> &res)
{
    return (
        c.size() == res.size()
        && std::equal(c.begin(), c.end(), res.begin()));
}


#endif /* VECTOR_SET_TEST_TEST_UTILS_HXX */
