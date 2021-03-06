#include "parentheses_parser.h"

#include <antlisp/lib/test/ut.h>

#include <sstream>


void testParenthesesRecursiveReader() {
    std::istringstream in("  (sum (* 2) (+ 5) 15) ");
    auto codeStream = AntLisp::InCodeStream(in);
    UT_ASSERT(
        codeStream.good()
    );
    auto token = std::string();
    auto parser0 = AntLisp::ParenthesesParser::fromCodeStream(codeStream);
    UT_ASSERT(
        not parser0.nextToken(token)
    );
    {
        auto parser1 = parser0.nextParser();
        UT_ASSERT(parser1);
        UT_ASSERT(
            parser1->nextToken(token)
        );
        UT_ASSERT_EQUAL(token, "sum");
        UT_ASSERT(
            not parser1->nextToken(token)
        );
        UT_ASSERT(
            not parser1->isLocked()
        );
        UT_ASSERT(
            not parser1->isEnd()
        );
        {
            auto parser2 = parser1->nextParser();
            UT_ASSERT(parser2);
            UT_ASSERT(
                parser1->isLocked()
            );
            UT_ASSERT(
                parser2->nextToken(token)
            );
            UT_ASSERT_EQUAL(token, "*");
            UT_ASSERT(
                parser2->nextToken(token)
            );
            UT_ASSERT_EQUAL(token, "2");
            UT_ASSERT(
                not parser2->nextToken(token)
            );
            UT_ASSERT(
                not parser2->nextParser()
            );
            parser2->close();
            UT_ASSERT(
                not parser2->good()
            );
            UT_ASSERT(
                parser2->isEnd()
            );
            UT_ASSERT(
                parser2->isLocked()
            );
        }
        UT_ASSERT(
            parser1->good()
        );
        UT_ASSERT(
            not parser1->isEnd()
        );
        UT_ASSERT(
            not parser1->isLocked()
        );
        UT_ASSERT(
            not parser1->nextToken(token)
        );
        {
            auto parser3 = parser1->nextParser();
            UT_ASSERT(parser3);
            UT_ASSERT(
                parser3->nextToken(token)
            );
            UT_ASSERT_EQUAL(token, "+");
            UT_ASSERT(
                parser3->nextToken(token)
            );
            UT_ASSERT_EQUAL(token, "5");
            UT_ASSERT(
                not parser3->nextToken(token)
            );
            UT_ASSERT(
                not parser3->nextParser()
            );
            parser3->close();
            UT_ASSERT(
                parser3->isEnd()
            );
            UT_ASSERT(
                not parser3->good()
            );
        }
        UT_ASSERT(
            not parser1->isEnd()
        );
        UT_ASSERT(
            not parser1->isLocked()
        );
        UT_ASSERT(
            parser1->nextToken(token)
        );
        UT_ASSERT_EQUAL(token, "15");
        parser1->close();
    }
    parser0.close();
}

void testParenthesesReaderCheck() {
    std::istringstream in("  (sum div)");
    auto codeStream = AntLisp::InCodeStream(in);
    UT_ASSERT(
        codeStream.good()
    );
    auto token = std::string();
    auto parser0 = AntLisp::ParenthesesParser::fromCodeStream(codeStream);
    UT_ASSERT(
        not parser0.nextToken(token)
    );
    {
        auto parser1 = parser0.nextParser();
        UT_ASSERT(parser1);
        parser1->nextToken(token);
        UT_ASSERT_EQUAL(token, "sum");
        UT_ASSERT_EXCEPTION_TYPE(
            parser1->close(),
            AntLisp::ParenthesesParser::Error
        );
    }
    parser0.close();
}

UT_LIST(
    RUN_TEST(testParenthesesRecursiveReader);
    RUN_TEST(testParenthesesReaderCheck);
);
