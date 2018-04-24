/*
 * Copyright 2017-2018 the QuadIron authors
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <cstdint>
#include <sstream>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "exceptions.h"
#include "uint160.h"

template <typename T>
std::string int_to_hex(T n)
{
    std::stringstream stream;

    stream << std::setfill('0') << std::setw(40) << std::hex << n;
    return stream.str();
}

TEST(UInt160Test, TestInitDefault) // NOLINT
{
    const kad::UInt160 n{};
    const kad::UInt160 zero(0u);

    // NOLINTNEXTLINE(hicpp-vararg)
    EXPECT_EQ(n, zero) << "default constructor return the value 0";
}

TEST(UInt160Test, TestInitFromInt) // NOLINT
{
    const uint64_t values[] = {0, 1, 255, 65000, 4000000, 10000000000};

    for (auto val : values) {
        const std::string result(kad::UInt160(val).to_string());
        const std::string expected(int_to_hex(val));

        // NOLINTNEXTLINE(hicpp-vararg)
        EXPECT_EQ(result, expected) << "testing init from " << val;
    }
}

TEST(UInt160Test, TestInitFromHex) // NOLINT
{
    const std::string hex("c544b5e4a1afcbb5d2de772d7a8df76f32557147");
    kad::UInt160 n(hex);

    // NOLINTNEXTLINE(hicpp-vararg)
    EXPECT_EQ(n.to_string(), hex) << "testing init from " << hex;

    ASSERT_THROW(kad::UInt160("deadbeef"), kad::LogicError)
        << "bad hex string size";

    ASSERT_THROW(
        kad::UInt160("One cannot step twice in the same river."),
        kad::Exception)
        << "bad string (not an hex string)";
}

TEST(UInt160Test, TestBitLength) // NOLINT
{
    const std::pair<kad::UInt160, int> testcases[] = {
        std::make_pair(0u, 0),
        std::make_pair(1u, 1),
        std::make_pair(2u, 2),
        std::make_pair(3u, 2),
        std::make_pair(4u, 3),
        std::make_pair(5u, 3),
        std::make_pair(6u, 3),
        std::make_pair(7u, 3),
        std::make_pair(
            kad::UInt160("8f0b49e7cdc5c120599cfe86886b622b2969e24f"), 160)};

    for (const auto& test : testcases) {
        // NOLINTNEXTLINE(hicpp-vararg)
        EXPECT_EQ(test.first.bit_length(), test.second)
            << "testing bit length of " << test.first;
    }
}

TEST(UInt160Test, TestBoolContext) // NOLINT
{
    const kad::UInt160 zero(0u);
    const kad::UInt160 n(42u);

    ASSERT_FALSE(bool(zero)) << "zero is false";
    ASSERT_TRUE(bool(n)) << "non zero is true";

    ASSERT_TRUE(!zero) << "!zero is true ";
    ASSERT_FALSE(!n) << "!non zero is false";

    ASSERT_FALSE(zero && zero) << "test false && false ";
    ASSERT_FALSE(zero && n) << "test false && true";
    ASSERT_FALSE(n && zero) << "test true && false";
    ASSERT_TRUE(n && n) << "test true && true ";

    ASSERT_FALSE(zero || zero) << "test false || false ";
    ASSERT_TRUE(zero || n) << "test false || true";
    ASSERT_TRUE(n || zero) << "test true || false";
    ASSERT_TRUE(n || n) << "test true || true ";
}

TEST(UInt160Test, TestOrdering) // NOLINT
{
    const kad::UInt160 a("3198e90e9149480c3d76354e1cbbdbff5c80b590");
    const kad::UInt160 b("97a149c721c981fe4e3a7440f6227ac4797bc238");

    ASSERT_TRUE(a == a) << "test ==";
    ASSERT_TRUE(a != b) << "test !=";

    ASSERT_TRUE(a < b) << "test <";
    ASSERT_TRUE(a <= b) << "test <=";

    ASSERT_FALSE(a > b) << "test >";
    ASSERT_FALSE(a >= b) << "test >=";

    ASSERT_TRUE(a <= a) << "test <= on itself";
    ASSERT_TRUE(a >= a) << "test >= on itself";
}

TEST(UInt160Test, TestBitwiseLogical) // NOLINT
{
    const kad::UInt160 a("6ebee938eda3024d35d74729d7475a6b826a4a70");
    const kad::UInt160 b("3af7f7bf61bd037eaee1b43bb895b57a21b4deb4");
    const kad::UInt160 a_and_b("2ab6e13861a1024c24c104299005106a00204a30");
    const kad::UInt160 a_or_b("7effffbfedbf037fbff7f73bffd7ff7ba3fedef4");
    const kad::UInt160 a_xor_b("54491e878c1e01339b36f3126fd2ef11a3de94c4");
    const kad::UInt160 not_a("914116c7125cfdb2ca28b8d628b8a5947d95b58f");

    ASSERT_EQ(a & b, a_and_b) << "test &"; // NOLINT(hicpp-vararg)
    ASSERT_EQ(a | b, a_or_b) << "test |";  // NOLINT(hicpp-vararg)
    ASSERT_EQ(a ^ b, a_xor_b) << "test ^"; // NOLINT(hicpp-vararg)
    ASSERT_EQ(~a, not_a) << "test ~";      // NOLINT(hicpp-vararg)
}
