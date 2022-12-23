//
// Sorted vector container.
//
// Copyright (c) 2022 kla-sch@gmx.de
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the “Software”), to deal in the Software without
// restriction, including without limitation the rights to use, copy,
// modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
// BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef VECTOR_SET_H
#define VECTOR_SET_H

#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>

#if __cplusplus >= 201402L // c++14
#    define VSET_CXX11_empty_constexpr constexpr
#else
#    define VSET_CXX11_empty_constexpr
#endif

/// Modes of this vectorset container.
enum class vectorset_mode : char {
    /// ordered content, unique values (set).
    unique_ordered,

    /// unordered content (vector).
    unordered
};


/**
 * A set based on `std::vector` container.
 *
 * The container operates in a unique ordered and an unordered
 * mode. In uniquely sorted mode, all elements are sorted and the
 * container behaves similar to a `std::set`. In unsorted mode,
 * however, the container behaves like a `std::vector`.
 *
 * You can toggle between the two modes, affecting the performance of
 * the container.
 */
template<
    class Key,
    class Compare = std::less<Key>,
    class Allocator = std::allocator<Key>
    >
class vectorset {
public:

    using vector_type = std::vector<Key, Allocator>;
    using key_type = Key;
    using value_type = Key;
    using size_type  = typename vector_type::size_type;
    using difference_type = typename vector_type::difference_type;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;

    using reference = value_type&;
    using const_reference = const value_type&;

    using pointer =
        typename std::allocator_traits<Allocator>::pointer;
    using const_pointer =
        typename std::allocator_traits<Allocator>::const_pointer;

    using iterator =
        typename vector_type::iterator;
    using const_iterator =
        typename vector_type::const_iterator;

    using reverse_iterator =
        typename vector_type::reverse_iterator;
    using const_reverse_iterator =
        typename vector_type::const_reverse_iterator;

private:
    /// container class with values.
    vector_type values;

    /// operating mode (unique ordered or unordered)
    vectorset_mode op_mode = vectorset_mode::unordered;

    /// compare method
    Compare comp;


    /**
     * Equal operator based on `comp`.
     *
     * @param lhv Left-hand value.
     * @param rhv Right-hand value.
     *
     * @return `true`, if `lhv==rhv`
     */
    bool
    comp_eq(const_reference lhv, const_reference rhv) const {
        return !(comp(lhv, rhv) || comp(rhv, lhv));
    }


    /**
     * Insert value into ordered vector.
     *
     * @param value Value to insert.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted.
     */
    template<class ValType>
    std::pair<iterator, bool>
    impl_insert_unique_ordered(ValType&& value) {
        auto iter =
            std::lower_bound(values.begin(), values.end(), value, comp);

        if (iter != values.end() && !comp(value, *iter)) {
            return std::make_pair(iter, false);
        }
        else {
            auto resIter = values.insert(iter, std::forward<ValType>(value));
            return std::make_pair(resIter, true);
        }
    }

    /**
     * Insert value into unordered vector.
     *
     * basicly a `push_back()`.
     *
     * @param value Value to insert.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted.
     */
    template<class ValType>
    std::pair<iterator, bool>
    impl_insert_unordered(ValType&& value) {
        values.push_back(std::forward<ValType>(value));
        return std::make_pair(std::prev(values.end()), true);
    }

    /**
     * Insert value at position (ordered vector).
     *
     * @param pos Position (hint).
     * @param value Value to add.
     *
     * @return Iterator to added value or iterator to already existing value.
     */
    template<class ValType>
    iterator
    impl_insert_unique_ordered(const_iterator pos, ValType&& value) {
        if (values.begin() != values.end()) {
            if ( (pos == values.begin() || comp(*(pos-1), value))
                 && (pos == values.end() || comp(value, *pos)))
            {
                return values.insert(pos, std::forward<ValType>(value));
            }

            if (pos != values.end() && comp_eq(*pos, value)) {
                auto distance = std::distance(values.cbegin(), pos);
                return std::next(values.begin(), distance);
            }
        }

        auto result = impl_insert_unique_ordered(std::forward<ValType>(value));

        return result.first;
    }


    /**
     * Insert value at position (unordered vector).
     *
     * @param pos Position.
     * @param value Value to add.
     *
     * @return Iterator to added value.
     */
    template<class ValType>
    iterator
    impl_insert_unordered(const_iterator pos, ValType&& value) {
        return values.insert(pos, std::forward<ValType>(value));
    }


    /**
     * Move value from end into position.
     *
     * Expects val_iter > pos.
     *
     * @param val_iter Position of value.
     * @param pos Position to move value into.
     */
    void
    move_into_position(iterator val_iter, const_iterator pos) {
        while (val_iter != pos) {
            std::swap(*(val_iter-1), *val_iter);
            --val_iter;
        }
    }


    /**
     * Helper to emplace new value (unique ordered).
     *
     * Precondition: value has to be stored as last element of `values`.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted.
     */
    std::pair<iterator, bool>
    impl_insert_emplace_unique_ordered() {
        auto last = std::prev(values.end());
        auto iter = std::lower_bound(values.begin(), last, *last, comp);

        if (iter == last) {
            // emplace at end
            return std::make_pair(last, true);
        }
        else if (!comp(*last, *iter)) {
            // value already exists
            values.pop_back();
            return std::make_pair(iter, false);
        }
        else {
            // move value into position
            move_into_position(last, iter);
            return std::make_pair(iter, true);
        }
    }


    /**
     * Emplace new value (unique ordered).
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted.
     */
    template<class... Args>
    std::pair<iterator, bool>
    impl_emplace_unique_ordered(Args&&... args) {
        values.emplace_back(std::forward<Args>(args)...);

        return impl_insert_emplace_unique_ordered();
    }


    /**
     * Emplace new value (unordered).
     *
     * @param args Constructor arguments.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted (always `true`).
     */
    template<class... Args>
    std::pair<iterator, bool>
    impl_emplace_unordered(Args&&... args) {
        values.emplace_back(std::forward<Args>(args)...);
        return std::make_pair(std::prev(values.end()), true);
    }


    /**
     * Emplace new value (ordered, with hint).
     *
     * @param hint Position in front of which the element is likely
     *             to be created.
     * @param args Constructor arguments.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted (always `true`).
     */
    template<class... Args>
    iterator
    impl_emplace_unique_ordered(const_iterator hint, Args&&... args) {
        auto hintDistance = std::distance(values.cbegin(), hint);
        values.emplace_back(std::forward<Args>(args)...);

        auto valIter = std::prev(values.end());
        auto hintIter = std::next(values.begin(), hintDistance);

        if (values.begin() != valIter) {
            // container was not empty

            if ( (   hintIter == values.begin()
                     || comp(*std::prev(hintIter), *valIter))
                 && (hintIter == valIter
                     || comp(*valIter, *hintIter)))
            {
                // hint is at the right spot
                move_into_position(valIter, hintIter);
                return hintIter;
            }

            if (hintIter != valIter && comp_eq(*hintIter, *valIter)) {
                // value exists at hint
                values.pop_back();
                return hintIter;
            }
        }

        auto result = impl_insert_emplace_unique_ordered();

        return result.first;
    }


    /**
     * Emplace new value (ordered, with hint).
     *
     * @param hint Position in front of which the element should be
     *             to be created.
     * @param args Constructor arguments.
     *
     * @return result pair: iterator to inserted value and flag, if
     *         values has been inserted (always `true`).
     */
    template<class... Args>
    iterator
    impl_emplace_unordered(const_iterator hint, Args&&... args) {
        return values.emplace(hint, std::forward<Args>(args)...);
    }


    /**
     * Erase on unique ordered vector.
     *
     * @param key Key value to remove.
     *
     * @return number of values removed (0 or 1).
     */
    size_type
    impl_erase_unique_ordered(const Key& key) {
        auto res_pair =
            std::equal_range(values.begin(), values.end(), key, comp);

        if (res_pair.first == res_pair.second) {
            return 0;
        }
        else {
            values.erase(res_pair.first);
            return 1;
        }
    }

    /**
     * Erase on unordered vector (remove all key values).
     *
     * @param key Key value(s) to remove.
     *
     * @return number of values removed.
     */
    size_type
    impl_erase_unordered(const Key& key) {
        auto rm_iter =
            std::remove_if(
                values.begin(), values.end(),
                [&key, this] (const Key &val){
                    return this->comp_eq(key, val);
                });
        auto rm_size = std::distance(rm_iter, values.end());

        values.erase(rm_iter, values.end());

        return rm_size;
    }


    /**
     * Find key in ordered container.
     */
    iterator
    impl_find_unique_ordered(const_reference key) {
        auto res_pair =
            std::equal_range(values.begin(), values.end(), key, comp);

        return (res_pair.first == res_pair.second ? end() : res_pair.first);
    }

    /**
     * Find key in ordered container.
     */
    const_iterator
    impl_find_unique_ordered(const_reference key) const {
        auto res_pair =
            std::equal_range(values.begin(), values.end(), key, comp);

        return (res_pair.first == res_pair.second ? end() : res_pair.first);
    }

    /**
     * Find x in ordered container.
     */
    template<class K>
    iterator
    impl_find_unique_ordered(const K &x) {
        auto res_pair = std::equal_range(values.begin(), values.end(), x);

        return (res_pair.first == res_pair.second ? end() : res_pair.first);
    }


    /**
     * Find x in ordered container (const).
     */
    template<class K>
    const_iterator
    impl_find_unique_ordered(const K &x) const {
        auto res_pair = std::equal_range(values.begin(), values.end(), x);

        return (res_pair.first == res_pair.second ? end() : res_pair.first);
    }

    /**
     * Find key in unordered container.
     */
    iterator
    impl_find_unordered(const_reference key) {
        auto find_comp =
            [&key, this](const Key &value) {
                return comp_eq(key, value);
            };

        return std::find_if(values.begin(), values.end(), find_comp);
    }

    /**
     * Find key in unordered container.
     */
    const_iterator
    impl_find_unordered(const_reference key) const {
        auto find_comp =
            [&key, this](const Key &value) {
                return comp_eq(key, value);
            };

        return std::find_if(values.begin(), values.end(), find_comp);
    }

    /**
     * Find x in unordered container.
     */
    template<class K>
    iterator
    impl_find_unordered(const K& x) {
        auto find_comp =
            [&x](const Key &value) {
                return !(x < value || value < x);
            };

        return std::find_if(values.begin(), values.end(), find_comp);
    }

    /**
     * Find x in unordered container.
     */
    template<class K>
    const_iterator
    impl_find_unordered(const K& x) const {
        auto find_comp =
            [&x](const Key &value) {
                return !(x < value || value < x);
            };

        return std::find_if(values.begin(), values.end(), find_comp);
    }


    /**
     * Count key in ordered container.
     */
    size_type
    impl_count_unique_ordered(const_reference key) const {
        auto res_pair =
            std::equal_range(values.begin(), values.end(), key, comp);

        return (res_pair.first == res_pair.second ? 0 : 1);
    }

    /**
     * Find x in ordered container (const).
     */
    template<class K>
    size_type
    impl_count_unique_ordered(const K &x) const {
        auto res_pair = std::equal_range(values.begin(), values.end(), x);

        return (res_pair.first == res_pair.second ? 0 : 1);
    }

    /**
     * Find key in unordered container.
     */
    size_type
    impl_count_unordered(const_reference key) const {
        auto count_comp =
            [&key, this](const Key &value) {
                return comp_eq(key, value);
            };

        return std::count_if(values.begin(), values.end(), count_comp);
    }

    /**
     * Count x in unordered container.
     */
    template<class K>
    size_type
    impl_count_unordered(const K& x) const {
        auto count_comp =
            [&x](const Key &value) {
                return !(x < value || value < x);
            };

        return std::count_if(values.begin(), values.end(), count_comp);
    }


    template<class Value, class AlgoComp>
    std::pair<iterator,iterator>
    impl_equal_range(
        const Value &value,
        AlgoComp algoComp)
    {
        if (op_mode == vectorset_mode::unordered) {
            throw std::logic_error(
                "equal_range is only available in"
                " vectorset_mode::unique_ordered");
        }

        return std::equal_range(values.begin(), values.end(), value, algoComp);
    }

    template<class Value, class AlgoComp>
    std::pair<const_iterator,const_iterator>
    impl_equal_range(
        const Value &value,
        AlgoComp algoComp)
        const
    {
        if (op_mode == vectorset_mode::unordered) {
            throw std::logic_error(
                "equal_range is only available in"
                " vectorset_mode::unique_ordered");
        }

        return std::equal_range(values.begin(), values.end(), value, algoComp);
    }

    template<class Value, class AlgoComp>
    iterator
    impl_bound_fn(const Value &value, bool lowerBound, AlgoComp algoComp)
    {
        if (op_mode == vectorset_mode::unordered) {
            throw std::logic_error(
                "lower_bound and upper_bound are only available"
                " in vectorset_mode::unique_ordered");
        }

        return
            (lowerBound
             ? std::lower_bound(values.begin(), values.end(), value, algoComp)
             : std::upper_bound(values.begin(), values.end(), value, algoComp));
    }

    template<class Value, class AlgoComp>
    const_iterator
    impl_bound_fn(const Value &value, bool lowerBound, AlgoComp algoComp) const
    {
        if (op_mode == vectorset_mode::unordered) {
            throw std::logic_error(
                "lower_bound and upper_bound are only available"
                " in vectorset_mode::unique_ordered");
        }

        return
            (lowerBound
             ? std::lower_bound(values.begin(), values.end(), value, algoComp)
             : std::upper_bound(values.begin(), values.end(), value, algoComp));
    }


    /**
     * Sort vector and remove duplicates.
     */
    void
    make_unique_ordered() {
        std::sort(values.begin(), values.end(), comp);
        auto rm_iter =
            unique(
                values.begin(), values.end(),
                [this](const Key &lhv, const Key &rhv) {
                    return this->comp_eq(lhv, rhv);
                });

        values.erase(rm_iter, values.end());
    }

public:

    /// @name Constructors

    /// @{

    /**
     * Default constructor.
     *
     * Constructs empty container.
     */
    vectorset() : comp(Compare()) { /* empty */ };

    /**
     * Construct empty container (comparator / allocator).
     *
     * @param comp Comparison function object to use for all
     *             comparisons of keys.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    explicit
    vectorset(const Compare& comp, const Allocator& alloc = Allocator())
        : values(alloc), comp(comp) { /* empty */ };

    /**
     * Constructor (allocator).
     *
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    explicit
    vectorset(const Allocator& alloc)
        : vectorset(Compare(), alloc) { /* empty */ };

    /**
     * Range constructor (comparator / allocator).
     *
     * Constructs the container with the contents of the range
     * [first, last).
     *
     * @param first Input interator to first value to insert.
     * @param last Input interator after last value to insert.
     * @param comp Comparison function object to use for all
     *             comparisons of keys.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    template<class InputIt>
    vectorset(
        InputIt first, InputIt last,
        const Compare& comp = Compare(),
        const Allocator& alloc = Allocator())
        : values(first, last, alloc), comp(comp) { /* empty */ };

    /**
     * Range constructor (allocator).
     *
     * Constructs the container with the contents of the range
     * [first, last).
     *
     * @param first Input interator to first value to insert.
     * @param last Input interator after last value to insert.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    template<class InputIt>
    vectorset(InputIt first, InputIt last, const Allocator& alloc)
        : vectorset(first, last, Compare(), alloc) { /* empty */ }

    /**
     * Copy constructor.
     *
     * @param other Another container to be used as source to initialize
     *              the elements of the container with.
     */
    vectorset(const vectorset& other)
        : vectorset()
        {
            if (this == &other) {
                return;
            }

            values = other.values;
            op_mode = other.op_mode;
        };

    /**
     * Move constructor.
     *
     * @param other Another container to be used as source to initialize
     *              the elements of the container with.
     */
    vectorset(vectorset&& other)
        : vectorset()
        {
            if (this == &other) {
                return;
            }

            values = std::move(other.values);
            op_mode = other.op_mode;
            other.op_mode = vectorset_mode::unordered;
        }


    /**
     * Move constructor with allocator.
     *
     * @param other Another container to be used as source to initialize
     *              the elements of the container with.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    vectorset(vectorset&& other, const Allocator& alloc)
        : vectorset(alloc)
        {
            if (this == &other) {
                return;
            }

            values = std::move(other.values);
            op_mode = other.op_mode;
        }

    /**
     * Initializer-list constructor (comparator / allocator).
     *
     * @param init Initializer-list to copy.
     * @param comp Comparison function object to use for all
     *             comparisons of keys.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    vectorset(
        std::initializer_list<value_type> init,
        const Compare& comp = Compare(),
        const Allocator& alloc = Allocator())
        : values(init, alloc), comp(comp) { /* empty */ }

    /**
     * Initializer-list constructor (allocator).
     *
     * @param init Initializer list to initialize the elements of
     *             the container with.
     * @param alloc Allocator to use for all memory allocations
     *              of this container.
     */
    vectorset(std::initializer_list<value_type> init, const Allocator& alloc )
        : vectorset(init, Compare(), alloc) { /* empty*/ }

    /// @}

    /// @name Miscellaneous

    /// @{


    /**
     * Get allocator associated with the container.
     *
     * @return allocator.
     */
    VSET_CXX11_empty_constexpr
    allocator_type
    get_allocator() const noexcept {
        return values.get_allocator();
    }


    /// @}

    /// @name Special

    /// @{


    /**
     * Set mode of container.
     *
     * @param op_mode New operation mode: `vectorset_mode::unordered` or
     *             `  vectorset_mode::unique_ordered`.
     */
    void
    set_mode(vectorset_mode op_mode) {
        if (this->op_mode == op_mode) {
            return;
        }

        if (op_mode == vectorset_mode::unordered) {
            this->op_mode = vectorset_mode::unordered;
            return;
        }

        make_unique_ordered();

        this->op_mode = vectorset_mode::unique_ordered;
    }


    /**
     * Get current mode of container.
     *
     * @return Mode: `vectorset_mode::unordered` or
     *         `vectorset_mode::unique_ordered`.
     */
    vectorset_mode
    get_mode() const noexcept {
        return op_mode;
    }

    /// @}


    /// @name Assignment Operators

    /// @{


    /**
     * Copy assignment operator.
     *
     * Replaces the contents with a copy of the contents of other.
     *
     * @param other Other instance to assign.
     *
     * @return Reference to this class.
     */
    vectorset&
    operator=(const vectorset& other) {
        if (this == &other) {
            return *this;
        }

        values = other.values;
        op_mode = other.op_mode;

        return *this;
    }


    /**
     * Move assignment operator.
     *
     * Replaces the contents with those of other using move semantics
     * (i.e. the data in other is moved from other into this
     * container).
     *
     * @param other Other instance to move.
     *
     * @return Reference to this class.
     */
    vectorset&
    operator=(vectorset&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        values = move(other.values);
        op_mode = other.op_mode;

        return *this;
    }

    /**
     * Initial assignment.
     *
     * Replaces the contents with those identified by initializer list.
     *
     * @param ilist Initializer-list to copy.
     *
     * @return Reference to this class.
     */
    vectorset&
    operator=(std::initializer_list<value_type> ilist) {
        copy(ilist.begin(), ilist.end(), back_inserter(values));
        if (op_mode == vectorset_mode::unique_ordered) {
            make_unique_ordered();
        }
    }

    /// @}

    /// @name Element access

    /// @{


    /**
     * Access specified element with bounds checking.
     *
     * @param pos Get reference to this position.
     *
     * @return Reference to element at position.
     */
    VSET_CXX11_empty_constexpr
    reference
    at(size_type pos) {
        return values.at(pos);
    }

    /**
     * Access specified element with bounds checking (const).
     *
     * @param pos Get reference to this position.
     *
     * @return Const reference to element at position.
     */
    VSET_CXX11_empty_constexpr
    const_reference
    at(size_type pos) const {
        return values.at(pos);
    }

    /**
     * Operator to access specified element.
     *
     * @param pos Get reference to this position.
     *
     * @return Reference to element at position.
     */
    VSET_CXX11_empty_constexpr
    reference
    operator[](size_type pos) {
        return values[pos];
    }

    /**
     * Operator to access specified element.
     *
     * @param pos Get reference to this position.
     *
     * @return Const reference to element at position.
     */
    VSET_CXX11_empty_constexpr
    const_reference
    operator[](size_type pos) const {
        return values[pos];
    }

    /**
     * Access the first element.
     *
     * @return Reference to first element.
     */
    VSET_CXX11_empty_constexpr
    reference
    front() {
        return values.front();
    }

    /**
     * Access the first element (const).
     *
     * @return Const reference to first element.
     */
    VSET_CXX11_empty_constexpr
    const_reference
    front() const {
        return values.front();
    }

    /**
     * Access the last element.
     *
     * @return Reference to last element.
     */
    VSET_CXX11_empty_constexpr
    reference
    back() {
        return values.back();
    }

    /**
     * Access the last element (const).
     *
     * @return Const reference to last element.
     */
    VSET_CXX11_empty_constexpr
    const_reference
    back() const {
        return values.back();
    }

    /**
     * Direct access to the underlying array
     *
     * @return Const pointer to array.
     */
    VSET_CXX11_empty_constexpr
    const Key*
    data() const noexcept {
        return values.data();
    }

    /**
     * Direct access to the underlying vector.
     *
     * @return Const reference to container..
     */
    VSET_CXX11_empty_constexpr
    const vector_type &
    get_vector() const noexcept {
        return values;
    }

    /// @}

    /// @name Iterators

    /// @{


    /// Iterator to the beginning.
    VSET_CXX11_empty_constexpr
    iterator
    begin() noexcept {
        return values.begin();
    };

    /// Iterator to the beginning (const).
    VSET_CXX11_empty_constexpr
    const_iterator
    begin() const noexcept {
        return cbegin();
    };

    /// Iterator to the beginning (const).
    VSET_CXX11_empty_constexpr
    const_iterator
    cbegin() const noexcept {
        return values.cbegin();
    };


    /// Iterator to the end.
    VSET_CXX11_empty_constexpr
    iterator
    end() noexcept {
        return values.end();
    }

    /// Iterator to the end (const).
    VSET_CXX11_empty_constexpr
    const_iterator
    end() const noexcept {
        return cend();
    }

    /// Iterator to the end (const).
    VSET_CXX11_empty_constexpr
    const_iterator
    cend() const noexcept {
        return values.cend();
    }


    /// Reverse iterator to the beginning.
    VSET_CXX11_empty_constexpr
    reverse_iterator
    rbegin() noexcept {
        return values.rbegin();
    }

    /// Reverse iterator to the beginning (const).
    VSET_CXX11_empty_constexpr
    const_reverse_iterator
    rbegin() const noexcept {
        return values.crbegin();
    }

    /// Reverse iterator to the beginning (const).
    VSET_CXX11_empty_constexpr
    const_reverse_iterator
    crbegin() const noexcept {
        return values.crbegin();
    }


    /// Reverse iterator to the end.
    VSET_CXX11_empty_constexpr
    reverse_iterator
    rend() noexcept {
        return values.rend();
    }

    /// Reverse iterator to the end (const).
    VSET_CXX11_empty_constexpr
    const_reverse_iterator
    rend() const noexcept {
        return values.crend();
    }

    /// Reverse iterator to the end (const).
    VSET_CXX11_empty_constexpr
    const_reverse_iterator
    crend() const noexcept {
        return values.crend();
    }


    /// @}

    /// @name Capacity

    /// @{


    /// Checks if the container has no elements.
    [[nodiscard]]
    bool
    empty() const noexcept {
        return values.empty();
    }

    /// Number of elements in the container.
    size_type
    size() const noexcept {
        return values.size();
    }

    /// Maximum possible number of elements .
    size_type
    max_size() const noexcept {
        return values.max_size();
    }

    /// Reserves storage.
    /// @param new_cap New capacity of the vector, in number of elements.
    VSET_CXX11_empty_constexpr
    void
    reserve(size_type new_cap) {
        values.reserve(new_cap);
    }

    /// Number of elements that can be held in currently allocated storage.
    VSET_CXX11_empty_constexpr
    size_type
    capacity() const noexcept {
        return values.capacity();
    }

    /// Reduces memory usage by freeing unused memory.
    VSET_CXX11_empty_constexpr
    void
    shrink_to_fit() {
        values.shrink_to_fit();
    }


    /// @}

    /// @name Modifiers

    /// @{


    /**
     * Clears the contents.
     *
     * Does not change mode of class.
     */
    void
    clear() noexcept {
        values.clear();
    }


    /**
     * Inserts new element (copy)
     *
     * * Insert the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   conains the value.
     * * If the container is unordered (`vectorset_mode::unordered`), the new
     *   values is always insert at the end (`push_back`).
     *
     * @param value Value to copy.
     *
     * @return Returns a pair consisting of an iterator to the
     *         inserted element and a bool value set to true if the
     *         insertion took place (always `true`, if
     *         `vectorset_mode::unordered`).
     */
    std::pair<iterator, bool>
    insert(const value_type& value) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_insert_unique_ordered(value)
                : impl_insert_unordered(value));
    }

    /**
     * Inserts new element (copy)
     *
     * * Insert the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains the value.
     * * If the container is unordered (`vectorset_mode::unordered`), the new
     *   value is always insert at the end (`push_back`).
     *
     * @param value Value to copy.
     *
     * @return Returns a pair consisting of an iterator to the
     *         inserted element and a bool value set to true if the
     *         insertion took place (always `true`, if
     *         `vectorset_mode::unordered`).
     */
    std::pair<iterator, bool>
    insert(value_type&& value) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_insert_unique_ordered(std::move(value))
                : impl_insert_unordered(std::move(value)));
    }

    /**
     * Inserts new element before position (copy).
     *
     * * Insert the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains the value. The position is used as a hint
     *   where to insert the value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value is inserted before the position.
     *
     * @param pos Iterator to the position before which the new
     *            element should be inserted.
     * @param value Value to copy.
     *
     * @return iterator to the inserted element.
     */
    iterator
    insert(const_iterator pos, const value_type& value) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_insert_unique_ordered(pos, value)
                : impl_insert_unordered(pos, value));
    }

    /**
     * Inserts new element before position (move).
     *
     * * Insert the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains the value. The position is used as a hint
     *   where to insert the value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value is inserted before the position.
     *
     * @param pos Iterator to the position before which the new
     *            element should be inserted.
     * @param value Value to copy.
     *
     * @return iterator to the inserted element.
     */
    iterator
    insert(const_iterator pos, value_type&& value) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_insert_unique_ordered(pos, std::move(value))
                : impl_insert_unordered(pos, std::move(value)));
    }

    /**
     * Inserts elements from range [first, last).
     *
     * * Insert the elements in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains a value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value appened at the end (`push_back()`).
     *
     * @param first Iterator to first element to insert.
     * @param last Iterator _after_ last element to insert.
     */
    template<class InputIt>
    void
    insert(InputIt first, InputIt last) {
        values.insert(values.end(), first, last);
        if (op_mode == vectorset_mode::unique_ordered) {
            make_unique_ordered();
        }
    }

    /**
     * Inserts elements from initializer list.
     *
     * * Insert the elements in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains a value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value appened at the end (`push_back()`).
     *
     * @param ilist Initializer-list to insert.
     */
    void
    insert(std::initializer_list<value_type> ilist) {
        values.insert(values.end(), ilist);
        if (op_mode == vectorset_mode::unique_ordered) {
            make_unique_ordered();
        }
    };


    /**
     * Constructs element in-place.
     *
     * * Create the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains the value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value is created at end.
     *
     * @param args Arguments to forward to the constructor of the element.
     *
     * @return Returns a pair consisting of an iterator to the
     *         new element and a bool value set to true if the
     *         insertion took place (always `true`, if
     *         `vectorset_mode::unordered`).
     */
    template<class... Args>
    std::pair<iterator,bool>
    emplace(Args&&... args) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_emplace_unique_ordered(std::forward<Args>(args)...)
                : impl_emplace_unordered(std::forward<Args>(args)...));
    }

    /**
     * Constructs element in-place using a hint.
     *
     * * Create the element in sorted container
     *   (`vectorset_mode::unique_ordered`), if the container does not
     *   already conains the value. The `hint` is used as a guess
     *   where to insert the value.
     * * If the container is unordered (`vectorset_mode::unordered`),
     *   the new value is inserted before the position of `hint`.
     *
     * @param hint Iterator to the position before which the new
     *             element will be inserted.
     * @param args Arguments to forward to the constructor of the element.
     */
    template <class... Args>
    iterator
    emplace_hint(const_iterator hint, Args&&... args) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_emplace_unique_ordered(hint, std::forward<Args>(args)...)
                : impl_emplace_unordered(hint, std::forward<Args>(args)...));
    }


    /**
     * Erase element at position.
     *
     * @param pos iterator to the element to remove.
     *
     * @return Iterator following the removed element.
     */
    VSET_CXX11_empty_constexpr
    iterator
    erase(const_iterator pos) {
        return values.erase(pos);
    }

    /**
     * Erase elements by iterator range.
     *
     * @param first Iterator to first element to remove.
     * @param last Iterator _after_ last element to remove.
     *
     * @return Iterator following the last removed element.
     */
    VSET_CXX11_empty_constexpr
    iterator
    erase(const_iterator first, const_iterator last) {
        return values.erase(first, last);
    }

    /**
     * Erase element by key.
     *
     * @param key Key value of the elements to remove.
     *
     * @return Number of elements removed. (0 or 1,
     *         if `vectorset_mode::unique_ordered`)
     */
    size_type
    erase(const Key& key) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_erase_unique_ordered(key)
                : impl_erase_unordered(key));
    }


    /**
     * Appends the given element value to the end of the container
     * (copy, only unordered container).
     *
     * @throws std::logic_error Method is used in mode
     *                          `vectorset_mode::unique_ordered`.
     *
     * @param value The value of the element to append.
     *
     */
    VSET_CXX11_empty_constexpr
    void
    push_back(const Key& value) {
        if (op_mode == vectorset_mode::unique_ordered) {
            throw std::logic_error(
                "push_back is only available in vectorset_mode::unordered");
        }

        values.push_back(value);
    };

    /**
     * Appends the given element value to the end of the container
     * (move, only unordered container).
     *
     * @throws std::logic_error Method is used in mode
     *                          `vectorset_mode::unique_ordered`.
     *
     * @param value The value of the element to append.
     *
     */
    VSET_CXX11_empty_constexpr
    void
    push_back(Key&& value) {
        if (op_mode == vectorset_mode::unique_ordered) {
            throw std::logic_error(
                "push_back is only available in vectorset_mode::unordered");
        }

        values.push_back(std::move(value));
    };

    /**
     * Removes the last element of the container.
     */
    VSET_CXX11_empty_constexpr
    void
    pop_back() {
        values.pop_back();
    }


    /**
     * Changes the number of elements stored
     *
     * @throws std::logic_error Method is used in mode
     *                          `vectorset_mode::unique_ordered` and
     *                          `count > size()`.
     *
     * @param count New size of the container.
     */
    VSET_CXX11_empty_constexpr
    void
    resize(size_type count) {
        if (op_mode == vectorset_mode::unique_ordered && count > size()) {
            throw std::logic_error(
                "resize cannot expand container in vectorset_mode::unordered");
        }

        values.resize(count);
    }

    /**
     * Changes the number of elements stored (only unordered container).
     *
     * @throws std::logic_error Method is used in mode
     *                          `vectorset_mode::unique_ordered`.
     *
     * @param count New size of the container.
     * @param value The value to initialize the new elements with.
     */
    VSET_CXX11_empty_constexpr
    void
    resize(size_type count, const value_type& value) {
        if (op_mode == vectorset_mode::unique_ordered) {
            throw std::logic_error(
                "resize cannot expand container in vectorset_mode::unordered");
        }

        values.resize(count, value);
    }


    /**
     * swap content.
     *
     * @param other Container to exchange the contents with.
     */
    void
    swap(vectorset& other) noexcept {
        values.swap(other.values);
        std::swap(op_mode, other.op_mode);
    }

    /// @}

    /// @name Lookup

    /// @{

    /**
     * Number of elements matching specific key.
     *
     * @param key Key value of the elements to count .
     *
     * @return number of elements with key (0 or 1 on
     *         `vectorset_mode::unique_ordered`)
     */
    size_type
    count(const Key& key) const {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_count_unique_ordered(key)
                : impl_count_unordered(key));
    }

    /**
     * Number of elements matching specific value.
     *
     * @param x Alternative value to compare to the keys.
     *
     * @return number of elements with key (0 or 1 on
     *         `vectorset_mode::unique_ordered`)
     */
    template<class K>
    size_type
    count(const K& x) const {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_count_unique_ordered(x)
                : impl_count_unordered(x));
    }


    /**
     * Find first element matches key.
     *
     * @param key Key value of the elements to count .
     *
     * @return iterator to matched element or `end()`, if no matching
     *         value is found.
     */
    iterator
    find(const Key& key) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_find_unique_ordered(key)
                : impl_find_unordered(key));
    }

    /**
     * Find first element matches key (const).
     *
     * @param key Key value of the element to search for.
     *
     * @return const iterator to matched element or `end()`, if no
     *         matching value is found.
     */
    const_iterator
    find(const Key& key) const {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_find_unique_ordered(key)
                : impl_find_unordered(key));
    }

    /**
     * Find first element matches key from other class.
     *
     * @param x A value of any type that can be transparently compared
     *          with a key (less Operator to Key needed).
     *
     * @return iterator to matched element or `end()`, if no
     *         matching value is found.
     */
    template<class K>
    iterator
    find(const K& x) {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_find_unique_ordered(x)
                : impl_find_unordered(x));
    }

    /**
     * Find first element matches key from other class (const).
     *
     * @param x A value of any type that can be transparently compared
     *          with a key (less Operator to Key needed).
     *
     * @return const iterator to matched element or `end()`, if no
     *         matching value is found.
     */
    template<class K>
    const_iterator
    find(const K& x) const {
        return (op_mode == vectorset_mode::unique_ordered
                ? impl_find_unique_ordered(x)
                : impl_find_unordered(x));
    }


    /**
     * Checks if the container contains element with specific key.
     *
     * @param key Key value of the element to search for.
     *
     * @return `true`, if container conains `key`.
     */
    bool
    contains(const Key& key) const {
        return (find(key) != end());
    }

    /**
     * Checks if the container contains element with specific value.
     *
     * @param x A value of any type that can be transparently compared
     *          with a key (less Operator to Key needed).
     *
     * @return `true`, if container conains `x`.
     */
    template<class K>
    bool
    contains(const K& x) const {
        return (find(x) != end());
    }


    /**
     * Range of elements matching a specific key.
     *
     * @param key Key value to compare the elements to.
     *
     * @return Two iterators, one pointing to the first element that
     *         is not less than key and another pointing to the first
     *         element greater than key.
     */
    std::pair<iterator,iterator>
    equal_range(const Key& key) {
        return impl_equal_range(key, comp);
    };

    /**
     * Range of elements matching a specific key (const).
     *
     * @param key Key value to compare the elements to.
     *
     * @return Two iterators, one pointing to the first element that
     *         is not less than key and another pointing to the first
     *         element greater than key.
     */
    std::pair<const_iterator,const_iterator>
    equal_range(const Key& key) const {
        return impl_equal_range(key, comp);
    }

#if __cplusplus >= 201402L
    /**
     * Range of elements matching a specific value.
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Two iterators, one pointing to the first element that
     *         is not less than the value and another pointing to the
     *         first element greater than the value.
     */
    template<class K>
    std::pair<iterator,iterator>
    equal_range(const K& x) {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_equal_range(x, x_comp);
    }

    /**
     * Range of elements matching a specific value (const).
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Two iterators, one pointing to the first element that
     *         is not less than the value and another pointing to the
     *         first element greater than the value.
     */
    template< class K >
    std::pair<const_iterator,const_iterator>
    equal_range(const K& x) const {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_equal_range(x, x_comp);
    }
#endif


    /**
     * Iterator to the first element not less than the given key.
     *
     * @param key Key to get lower bound iterator for.
     *
     * @return Iterator to the first element not less than the given key.
     */
    iterator
    lower_bound(const Key& key) {
        return impl_bound_fn(key, true, comp);
    }

    /**
     * Iterator to the first element not less than the given key (const).
     *
     * @param key Key to get lower bound iterator for.
     *
     * @return Iterator to the first element not less than the given key.
     */
    const_iterator
    lower_bound(const Key& key) const {
        return impl_bound_fn(key, true, comp);
    }

#if __cplusplus >= 201402L
    /**
     * Iterator to the first element not less than the given value.
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Iterator to the first element not less than the given key.
     */
    template<class K>
    iterator
    lower_bound(const K& x) {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_bound_fn(x, true, x_comp);
    }

    /**
     * Iterator to the first element not less than the given value (const).
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Iterator to the first element not less than the given key.
     */
    template<class K>
    const_iterator
    lower_bound(const K& x) const {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_bound_fn(x, true, x_comp);
    }
#endif


    /**
     * Iterator to the first element greater than the given key.
     *
     * @param key Key to get upper bound iterator for.
     *
     * @return Iterator to the first element greater than the given key.
     */
    iterator
    upper_bound(const Key& key) {
        return impl_bound_fn(key, false, comp);
    }

    /**
     * Iterator to the first element greater than the given key (const).
     *
     * @param key Key to get upper bound iterator for.
     *
     * @return Iterator to the first element greater than the given key.
     */
    const_iterator
    upper_bound(const Key& key) const {
        return impl_bound_fn(key, false, comp);
    }

#if __cplusplus >= 201402L
    /**
     * Iterator to the first element greater than the given value.
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Iterator to the first element greater than the given value.
     */
    template<class K>
    iterator
    upper_bound(const K& x) {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_bound_fn(x, false, x_comp);
    }

    /**
     * Iterator to the first element greater than the given value. (const)
     *
     * Requires *C++14* or higher.
     *
     * @param x Alternative value that can be compared to Key.
     *
     * @return Iterator to the first element greater than the given value.
     */
    template<class K>
    const_iterator
    upper_bound(const K& x) const {
        auto x_comp =
            [](const auto &lhv, const auto &rhv) {
                return (lhv < rhv);
            };
        return impl_bound_fn(x, false, x_comp);
    }
#endif

    /// @}

    /// @name Observers

    /// @{

    /**
     * Get function object that compares the keys.
     *
     * @return Function object that compares the keys.
     */
    key_compare
    key_comp() const {
        return comp;
    }

    /**
     * Get function object that compares the values.
     *
     * It is the same as `key_comp.`
     *
     * @return Function object that compares the values.
     */
    value_compare
    value_comp() const {
        return comp;
    }

    /// @}

}; // class vectorset




#if __cplusplus >= 202002L // c++20

/**
 * Three way compare of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return Comparison category type (`std::lexicographical_compare_three_way`).
 */
template<class T, class Compare, class Alloc>
constexpr auto
operator<=>(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() <=> rhs.get_vector());
}


/**
 * Equal operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs == rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
constexpr bool
operator==(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() == rhs.get_vector());
}

#else // until c++20, __cplusplus < 202002L

/**
 * Equal operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs == rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator==(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() == rhs.get_vector());
}

/**
 * Not equal operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs != rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator!=(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() != rhs.get_vector());
}

/**
 * Less operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs < rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator<(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() < rhs.get_vector());
}

/**
 * Less or equal operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs <= rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator<=(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() <= rhs.get_vector());
}

/**
 * Greater operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs > rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator>(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() > rhs.get_vector());
}

/**
 * Greater or equal operator of `vectorset`.
 *
 * @param lhs Left hand side.
 * @param rhs Right hand side.
 *
 * @return `true` if `lhs >= rhs` (`std::lexicographical_compare`).
 */
template<class T, class Compare, class Alloc>
bool
operator>=(
    const vectorset<T, Compare, Alloc>& lhs,
    const vectorset<T, Compare, Alloc>& rhs)
{
    return (lhs.get_vector() >= rhs.get_vector());
}

#endif // __cplusplus < 202002L

#endif /* VECTOR_SET_H */
