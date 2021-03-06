#include "cell_in_string.h"

#include <antlisp/lib/test/ut.h>

#include <iostream>


void test_tryFromString_number() {
    auto dst = AntLisp::tryCellFromString("12");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::Integer>(),
        12
    );
    dst = AntLisp::tryCellFromString("-511");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::Integer>(),
        -511
    );
    dst = AntLisp::tryCellFromString("-0911");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::Integer>(),
        -911
    );
    dst = AntLisp::tryCellFromString("0587");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::Integer>(),
        587
    );
    dst = AntLisp::tryCellFromString("1.01");
    UT_ASSERT(dst);
    UT_ASSERT(
        dst->is<AntLisp::Float>()
    );
    UT_ASSERT_EQUAL(
        static_cast<int>(dst->as<AntLisp::Float>()),
        1
    );
    dst = AntLisp::tryCellFromString("-2.01");
    UT_ASSERT(dst);
    UT_ASSERT(
        dst->is<AntLisp::Float>()
    );
    UT_ASSERT_EQUAL(
        static_cast<int>(dst->as<AntLisp::Float>()),
        -2
    );
    dst = AntLisp::tryCellFromString("0.02");
    UT_ASSERT(dst);
    UT_ASSERT(
        dst->is<AntLisp::Float>()
    );
    UT_ASSERT(
        dst->as<AntLisp::Float>() > 0.019
    );
    UT_ASSERT(
        dst->as<AntLisp::Float>() < 0.021
    );
    dst = AntLisp::tryCellFromString(".02");
    UT_ASSERT(dst);
    UT_ASSERT(
        dst->is<AntLisp::Float>()
    );
    UT_ASSERT(
        dst->as<AntLisp::Float>() > 0.019
    );
    UT_ASSERT(
        dst->as<AntLisp::Float>() < 0.021
    );
}

void test_tryFromString_string() {
    auto dst = AntLisp::tryCellFromString("\" abc \"");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::StringType>(),
        AntLisp::StringType{" abc "}
    );
}

void test_tryFromString_simple_char() {
    auto dst = AntLisp::tryCellFromString("#\\b");
    UT_ASSERT(dst);
    UT_ASSERT_EQUAL(
        dst->as<AntLisp::Symbol>(),
        'b'
    );
}

UT_LIST(
    RUN_TEST(test_tryFromString_number);
    RUN_TEST(test_tryFromString_string);
    RUN_TEST(test_tryFromString_simple_char);
);
