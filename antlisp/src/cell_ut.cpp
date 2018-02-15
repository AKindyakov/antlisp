#include "cell.h"

#include "ut.h"

#include <iostream>

void nilEqualTest() {
    UT_ASSERT_EQUAL(
        AntLisp::Cell::nil(), AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        AntLisp::Cell{AntLisp::Nil{}}, AntLisp::Cell::nil()
    );
    UT_ASSERT_EQUAL(
        AntLisp::Nil{}, AntLisp::Nil{}
    );
}

template<typename T>
void checkTagTempl() {
    auto v = AntLisp::Cell(T{});
    UT_ASSERT_EQUAL(
        static_cast<int>(v.tag()),
        static_cast<int>(AntLisp::Cell::tagOf<T>())
    );
}

void cellCheckTypeTag() {
    checkTagTempl<AntLisp::Nil>();
    checkTagTempl<AntLisp::Integer>();
    checkTagTempl<AntLisp::Float>();
    checkTagTempl<AntLisp::Symbol>();
    checkTagTempl<AntLisp::StringPtr>();
    checkTagTempl<AntLisp::ConsPtr>();
    checkTagTempl<AntLisp::FunctionDefinitionPtr>();
    checkTagTempl<AntLisp::ExtFunctionPtr>();
}

void cellGetError() {
    UT_ASSERT_EXCEPTION_TYPE(
        AntLisp::Cell::nil().get<AntLisp::Integer>(),
        AntLisp::Cell::BadGetError
    );
}

void test_tryFromString_number() {
    auto dst = AntLisp::Cell{};
    UT_ASSERT(
        AntLisp::tryFromString(dst, "12")
    );
    UT_ASSERT_EQUAL(
        dst.get<AntLisp::Integer>(),
        12
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "-511")
    );
    UT_ASSERT_EQUAL(
        dst.get<AntLisp::Integer>(),
        -511
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "-0911")
    );
    UT_ASSERT_EQUAL(
        dst.get<AntLisp::Integer>(),
        -911
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "0587")
    );
    UT_ASSERT_EQUAL(
        dst.get<AntLisp::Integer>(),
        587
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "1.01")
    );
    UT_ASSERT(
        dst.is<AntLisp::Float>()
    );
    UT_ASSERT_EQUAL(
        static_cast<int>(dst.get<AntLisp::Float>()),
        1
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "-2.01")
    );
    UT_ASSERT(
        dst.is<AntLisp::Float>()
    );
    UT_ASSERT_EQUAL(
        static_cast<int>(dst.get<AntLisp::Float>()),
        -2
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, "0.02")
    );
    UT_ASSERT(
        dst.is<AntLisp::Float>()
    );
    UT_ASSERT(
        dst.get<AntLisp::Float>() > 0.019
    );
    UT_ASSERT(
        dst.get<AntLisp::Float>() < 0.021
    );
    UT_ASSERT(
        AntLisp::tryFromString(dst, ".02")
    );
    UT_ASSERT(
        dst.is<AntLisp::Float>()
    );
    UT_ASSERT(
        dst.get<AntLisp::Float>() > 0.019
    );
    UT_ASSERT(
        dst.get<AntLisp::Float>() < 0.021
    );
}

void test_tryFromString_string() {
    auto dst = AntLisp::Cell{};
    UT_ASSERT(
        AntLisp::tryFromString(dst, "\" abc \"")
    );
    UT_ASSERT_EQUAL(
        *dst.get<AntLisp::StringPtr>().get(),
        " abc "
    );
}

void test_tryFromString_simple_char() {
    auto dst = AntLisp::Cell{};
    UT_ASSERT(
        AntLisp::tryFromString(dst, "#\\b")
    );
    UT_ASSERT_EQUAL(
        dst.get<AntLisp::Symbol>(),
        'b'
    );
}

UT_LIST(
    nilEqualTest();
    cellCheckTypeTag();
    cellGetError();
    test_tryFromString_number();
    test_tryFromString_string();
    test_tryFromString_simple_char();
);
