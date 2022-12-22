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

#ifndef STRING_GENERATOR_H
#define STRING_GENERATOR_H 1

#include <cstdlib>
#include <ctime>
#include <string>

#include <iostream>

/// Generate random strings.
struct StringGenerator {

    /// Default-Constructor.
    StringGenerator() {
        time_t now;
        time(&now);

        std::srand(now);
    }


    /**
     * Get random string.
     *
     * @param maxlen Maxmim length of string.
     *
     * @return Random string.
     */
    std::string
    get(size_t maxlen) {
        size_t randLen = (std::rand() * double(maxlen) / RAND_MAX);

        std::string result;
        result.reserve(randLen);

        for (size_t i=0; i < randLen; i++) {
            char randChar = (std::rand() * 95.0 / RAND_MAX) + 32;
            result += randChar;
        }

        return result;
    }

    /**
     * Get multiple random strings.
     *
     * @param maxlen Maxmim length of strings.
     * @param nStrings Number of strings to generate.
     * @param oIter Output-Iterator to store strings.
     */
    template<typename OIter>
    void
    get(size_t maxlen, size_t nStrings, OIter oIter) {
        for (size_t i=0; i < nStrings; i++) {
            *oIter = get(maxlen);
            oIter++;
        }
    }
};

#endif /* STRING_GENERATOR_H */
