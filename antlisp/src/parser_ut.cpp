#include "ut.h"

#include "parser.h"

#include <sstream>


void testInCodeStream() {
    std::istringstream in("  (sum 1.23 (* 2 3)) ");
    auto code = AntLisp::InCodeStream(in);
    auto token = std::string();
    UT_ASSERT(
        !code.nextToken(token)
    );
    UT_ASSERT(
        code.nextToken(token)
    );
    UT_ASSERT_EQUAL(
        token, "sum"
    );
    UT_ASSERT_EQUAL(
        code.nextToken(), "1.23"
    );
    UT_ASSERT_EXCEPTION_TYPE(
        code.nextToken(), AntLisp::InCodeStream::Error
    );
    UT_ASSERT_EQUAL(
        code.pCount(), 2
    );
    UT_ASSERT_EQUAL(
        code.nextToken(), "*"
    );
    UT_ASSERT_EQUAL(
        code.nextToken(), "2"
    );
    UT_ASSERT_EQUAL(
        code.nextToken(), "3"
    );
    UT_ASSERT_EQUAL(
        code.pCount(), 0
    );
}

void testParenthesesRecursiveReader() {
    std::istringstream in("  (sum (* 2) (+ 5) 15) ");
    auto reader = AntLisp::InCodeStream(in);
    UT_ASSERT(
        reader.good()
    );
    auto parser1 = AntLisp::ParenthesesParser::fromCodeStream(reader);
    UT_ASSERT(
        reader.good()
    );
    auto token = std::string();
    UT_ASSERT(
        parser1.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "sum");
    UT_ASSERT(
        !parser1.nextToken(token)
    );
    UT_ASSERT(
        reader.good()
    );
    UT_ASSERT(
        !parser1.good()
    );
    UT_ASSERT(
        parser1.isLocked()
    );

    auto parser2 = parser1.nextParser();
    UT_ASSERT(
        parser2.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "*");
    UT_ASSERT(
        parser2.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "2");
    UT_ASSERT(
        !parser2.good()
    );

    UT_ASSERT(
        parser1.good()
    );
    UT_ASSERT(
        !parser1.isLocked()
    );
    UT_ASSERT(
        !parser1.nextToken(token)
    );

    auto parser3 = parser1.nextParser();
    UT_ASSERT(
        parser3.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "+");
    UT_ASSERT(
        parser3.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "5");
    UT_ASSERT(
        !parser3.good()
    );

    UT_ASSERT(
        parser1.good()
    );
    UT_ASSERT(
        !parser1.isLocked()
    );
    UT_ASSERT(
        parser1.nextToken(token)
    );
    UT_ASSERT_EQUAL(token, "15");
}

UT_LIST(
    testInCodeStream();
    testParenthesesRecursiveReader();
);