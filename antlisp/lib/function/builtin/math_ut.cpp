#include "math.h"

#include <antlisp/lib/test/ut.h>

#include <iostream>


void test_sum() {
    auto sum = AntLisp::Builtin::Sum();
    UT_ASSERT_EQUAL(
        sum.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(192),
                AntLisp::Cell::integer(168),
                AntLisp::Cell::integer(1),
                AntLisp::Cell::integer(1),
            }
        ),
        AntLisp::Cell::integer(192 + 168 + 1 + 1)
    );
    {
        auto out = sum.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(92),
                AntLisp::Cell::real(16.99),
            }
        ).get<AntLisp::Float>();
        UT_ASSERT(out < 16 + 1.0 + 92);
        UT_ASSERT(out > 16 + 0.98 + 92);
    }
    {
        auto out = sum.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('$'),
                AntLisp::Cell::string("in the"),
                AntLisp::Cell::symbol(' '),
                AntLisp::Cell::string("dark"),
                AntLisp::Cell::symbol('$'),
            }
        );
        UT_ASSERT_EQUAL(
            *out.get<AntLisp::StringPtr>(),
            std::string("$in the dark$")
        );
        UT_ASSERT_EQUAL(
            out,
            AntLisp::Cell::string("$in the dark$")
        );
    }
}

void test_multiplication() {
    auto m = AntLisp::Builtin::Multiplication();
    UT_ASSERT_EQUAL(
        m.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(12),
                AntLisp::Cell::integer(-6),
            }
        ),
        AntLisp::Cell::integer(12 * -6)
    );
    UT_ASSERT_EQUAL(
        m.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('s'),
                AntLisp::Cell::integer(1),
            }
        ),
        AntLisp::Cell::string("s")
    );
    UT_ASSERT_EQUAL(
        m.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('s'),
                AntLisp::Cell::integer(3),
            }
        ),
        AntLisp::Cell::string("sss")
    );
    UT_ASSERT_EQUAL(
        m.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("Basic"),
                AntLisp::Cell::integer(2),
            }
        ),
        AntLisp::Cell::string("BasicBasic")
    );
    UT_ASSERT_EQUAL(
        m.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("welcomE"),
                AntLisp::Cell::integer(1),
            }
        ),
        AntLisp::Cell::string("welcomE")
    );
}

void test_less() {
    auto less = AntLisp::Builtin::Less();
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(86),
                AntLisp::Cell::integer(64),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(32),
                AntLisp::Cell::integer(64),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::real(192.168),
                AntLisp::Cell::real(1.155),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::real(2.16),
                AntLisp::Cell::real(10.7),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(2),
                AntLisp::Cell::real(10.7),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::real(-2.66),
                AntLisp::Cell::integer(-2),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("aa"),
                AntLisp::Cell::string("aaa"),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("abc"),
                AntLisp::Cell::string("abc"),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('a'),
                AntLisp::Cell::symbol('b'),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('c'),
                AntLisp::Cell::symbol('b'),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('a'),
                AntLisp::Cell::symbol('b'),
                AntLisp::Cell::symbol('c'),
                AntLisp::Cell::symbol('d'),
                AntLisp::Cell::symbol('e'),
                AntLisp::Cell::symbol('y'),
                AntLisp::Cell::symbol('z'),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(10),
                AntLisp::Cell::integer(20),
                AntLisp::Cell::integer(30),
                AntLisp::Cell::integer(30),
                AntLisp::Cell::integer(40),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        less.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('1'),
                AntLisp::Cell::symbol('2'),
                AntLisp::Cell::symbol('3'),
                AntLisp::Cell::symbol('3'),
                AntLisp::Cell::symbol('4'),
            }
        ),
        AntLisp::Cell::nil()
    );
}

void test_equality() {
    auto eq = AntLisp::Builtin::Equality();
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(48),
                AntLisp::Cell::integer(48),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(155),
                AntLisp::Cell::integer(-155),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('t'),
                AntLisp::Cell::symbol('t'),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('a'),
                AntLisp::Cell::symbol('A'),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("keymaster"),
                AntLisp::Cell::string("keymaster"),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::string("key master"),
                AntLisp::Cell::string("keymaster"),
            }
        ),
        AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('t'),
                AntLisp::Cell::symbol('t'),
                AntLisp::Cell::symbol('t'),
                AntLisp::Cell::symbol('t'),
                AntLisp::Cell::symbol('t'),
            }
        ),
        AntLisp::Cell::t()
    );
    UT_ASSERT_EQUAL(
        eq.instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::symbol('u'),
                AntLisp::Cell::symbol('u'),
                AntLisp::Cell::symbol('*'),
                AntLisp::Cell::symbol('u'),
            }
        ),
        AntLisp::Cell::nil()
    );
}

void test_allMathBuiltinFunctions() {
    auto space = AntLisp::Namespace{};
    AntLisp::Builtin::allMathFunctions(space);
    const auto sumPtr = space.at("+").get<AntLisp::FunctionPtr>();
    UT_ASSERT_EQUAL(
        sumPtr->instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(17),
                AntLisp::Cell::integer(16),
            }
        ),
        AntLisp::Cell::integer(17 + 16)
    );
    const auto lessPtr = space.at("<").get<AntLisp::FunctionPtr>();
    UT_ASSERT_EQUAL(
        lessPtr->instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::real(1.29),
                AntLisp::Cell::real(1.31),
            }
        ),
        AntLisp::Cell::t()
    );
    const auto eqPtr = space.at("=").get<AntLisp::FunctionPtr>();
    UT_ASSERT_EQUAL(
        eqPtr->instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::real(1.29),
                AntLisp::Cell::real(1.29),
            }
        ),
        AntLisp::Cell::t()
    );
    const auto mPtr = space.at("*").get<AntLisp::FunctionPtr>();
    UT_ASSERT_EQUAL(
        mPtr->instantCall(
            AntLisp::Arguments{
                AntLisp::Cell::integer(6),
                AntLisp::Cell::integer(29),
            }
        ),
        AntLisp::Cell::integer(6 * 29)
    );
}

UT_LIST(
    RUN_TEST(test_sum);
    RUN_TEST(test_multiplication);
    RUN_TEST(test_equality);
    RUN_TEST(test_less);
    RUN_TEST(test_allMathBuiltinFunctions);
);
