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


#include <benchmark/benchmark.h>
#include "StringGenerator.h"

#include "../vectorset.h"

#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <string>
#include <chrono>

using namespace std;

using TextData = vector<string>;
using TextDataIter = vector<pair<size_t,size_t>>;

static vector<pair<size_t, TextData>> randomDataCache;
static vector<pair<size_t, TextDataIter>> randomFindCache;

static TextData::iterator firstData;
static TextData::iterator lastData;

static vector<pair<size_t, TextDataIter> >::iterator randomStrings;
static TextDataIter::iterator nextRandom;

static
const string &
getRandomVal() {
    const auto &result =
        randomDataCache[nextRandom->first]
        .second[nextRandom->second];

    ++nextRandom;
    if (nextRandom == randomStrings->second.end()) {
        nextRandom = randomStrings->second.begin();
    }

    return result;
}

static set<string> setData;
static unordered_set<string> unorderedSetData;
static vectorset<string> vectorSetData;
static StringGenerator gen;


void
setupData(const benchmark::State& state) {
    size_t textLen = state.range(1);
    size_t numElems = state.range(0);

    auto dataIter =
        find_if(
            randomDataCache.begin(), randomDataCache.end(),
            [textLen](const auto &vPair) {
                return (vPair.first == textLen);
            });
    if (dataIter == randomDataCache.end()) {
        // New storage for string length `textLen`
        randomDataCache.push_back(make_pair(textLen, TextData{}));
        dataIter = prev(randomDataCache.end());
    }
    auto &dataCache = dataIter->second;

    auto newMinCapacity = max(numElems, size_t(1 << 10));
    if (dataCache.capacity() < newMinCapacity) {
        dataCache.reserve(newMinCapacity);
    }

    if (dataCache.size() < newMinCapacity) {
        gen.get(
            textLen, newMinCapacity-dataCache.size(),
            back_inserter(dataCache));
    }

    firstData = dataCache.begin();
    lastData = next(dataCache.begin(), numElems);

    // Random data:
    auto dataCacheIdx =
        std::distance(randomDataCache.begin(), dataIter);
    auto findIter =
        find_if(
            randomFindCache.begin(), randomFindCache.end(),
            [numElems](const auto &vPair) {
                return (vPair.first == numElems);
            });
    if (findIter == randomFindCache.end()) {
        randomFindCache.push_back(make_pair(numElems, TextDataIter{}));
        findIter = prev(randomFindCache.end());

        // Generate 2 x numElems, but at least 1024 random string positions.
        auto newFindCap = max(2 * numElems, size_t(1 << 10));
        findIter->second.reserve(newFindCap);
        while (newFindCap > 0) {
            size_t idx = rand() * double(numElems) / RAND_MAX;
            findIter->second.push_back(make_pair(dataCacheIdx, idx));
            --newFindCap;
        }
    }

    randomStrings = findIter;
    nextRandom = randomStrings->second.begin();
}


void
teardownData(const benchmark::State& state) {
}


void
setupSet(const benchmark::State& state) {
    setupData(state);
    setData.insert(firstData, lastData);
}

void
teardownSet(const benchmark::State& state) {
    setData.clear();
    teardownData(state);
}


void
setupUnorderedSet(const benchmark::State& state) {
    setupData(state);
    unorderedSetData.insert(firstData, lastData);
}

void
teardownUnorderedSet(const benchmark::State& state) {
    unorderedSetData.clear();
    teardownData(state);
}


void
setupVectorSet(const benchmark::State& state) {
    setupData(state);
    vectorSetData.clear();
    vectorSetData.set_mode(vectorset_mode::unordered);
    vectorSetData.insert(firstData, lastData);
    vectorSetData.set_mode(vectorset_mode::unique_ordered);
}

void
teardownVectorSet(const benchmark::State& state) {
    unorderedSetData.clear();
    teardownData(state);
}



static void
construct_set(benchmark::State& state) {
    for (auto _ : state) {
        set<string> test(firstData, lastData);
        benchmark::DoNotOptimize(test);
    }
}

static void
construct_unordered_set(benchmark::State& state) {
    for (auto _ : state) {
        unordered_set<string> test(firstData, lastData);
        benchmark::DoNotOptimize(test);
    }
}


static void
construct_vectorset_unordered(benchmark::State& state) {
    for (auto _ : state) {
        vectorset<string> test(firstData, lastData);
        test.set_mode(vectorset_mode::unordered);
        benchmark::DoNotOptimize(test);
    }
}

static void
construct_vectorset_unique_ordered(benchmark::State& state) {
    for (auto _ : state) {
        vectorset<string> test(firstData, lastData);
        test.set_mode(vectorset_mode::unique_ordered);
        benchmark::DoNotOptimize(test);
    }
}

BENCHMARK(construct_vectorset_unordered)
->Setup(setupData)
->Teardown(teardownData)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(construct_vectorset_unique_ordered)
->Setup(setupData)
->Teardown(teardownData)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(construct_set)
->Setup(setupData)
->Teardown(teardownData)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(construct_unordered_set)
->Setup(setupData)
->Teardown(teardownData)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});




static void
iter_set(benchmark::State& state) {
    for (auto _ : state) {
        for (auto &s : setData) {
            benchmark::DoNotOptimize(s.c_str());
        }
    }
}

static void
iter_unordered_set(benchmark::State& state) {
    for (auto _ : state) {
        for (auto &s : unorderedSetData) {
            benchmark::DoNotOptimize(s.c_str());
        }
    }
}


static void
iter_vectorset_ordered(benchmark::State& state) {
    for (auto _ : state) {
        for (auto &s : vectorSetData) {
            benchmark::DoNotOptimize(s.c_str());
        }
    }
}



BENCHMARK(iter_vectorset_ordered)
->Setup(setupVectorSet)
->Teardown(teardownVectorSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(iter_set)
->Setup(setupSet)
->Teardown(teardownSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});


BENCHMARK(iter_unordered_set)
->Setup(setupUnorderedSet)
->Teardown(teardownUnorderedSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});


static void
find_set(benchmark::State& state) {
    for (auto _ : state) {
        auto strIter = setData.find(getRandomVal());
        benchmark::DoNotOptimize(strIter);
    }
}

static void
find_unordered_set(benchmark::State& state) {
    for (auto _ : state) {
        auto strIter = unorderedSetData.find(getRandomVal());
        benchmark::DoNotOptimize(strIter);
    }
}


static void
find_vectorset_ordered(benchmark::State& state) {
    for (auto _ : state) {
        auto strIter = vectorSetData.find(getRandomVal());
        benchmark::DoNotOptimize(strIter);
    }
}


BENCHMARK(find_vectorset_ordered)
->Setup(setupVectorSet)
->Teardown(teardownVectorSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(find_set)
->Setup(setupSet)
->Teardown(teardownSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});


BENCHMARK(find_unordered_set)
->Setup(setupUnorderedSet)
->Teardown(teardownUnorderedSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});


static void
insert_erase_set(benchmark::State& state) {
    for (auto _ : state) {
        setData.insert(getRandomVal());
        setData.erase(getRandomVal());
    }
}

static void
insert_erase_unordered_set(benchmark::State& state) {
    for (auto _ : state) {
        unorderedSetData.insert(getRandomVal());
        unorderedSetData.erase(getRandomVal());
    }
}


static void
insert_erase_vectorset_ordered(benchmark::State& state) {
    for (auto _ : state) {
        vectorSetData.insert(getRandomVal());
        vectorSetData.erase(getRandomVal());
    }
}


BENCHMARK(insert_erase_vectorset_ordered)
->Setup(setupVectorSet)
->Teardown(teardownVectorSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});

BENCHMARK(insert_erase_set)
->Setup(setupSet)
->Teardown(teardownSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});


BENCHMARK(insert_erase_unordered_set)
->Setup(setupUnorderedSet)
->Teardown(teardownUnorderedSet)
->ArgsProduct({
        {1<<4, 1<<8, 1<<12, 1<<16, 1<<20},
        {1<<8}});



BENCHMARK_MAIN();
