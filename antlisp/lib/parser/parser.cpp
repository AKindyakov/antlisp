#include "parser.h"

#include "cell_in_string.h"
#include "parentheses_parser.h"

#include <antlisp/lib/util/string.h>

#include <iterator>


namespace AntLisp {

namespace {

class ConstructionParser {
public:
    explicit ConstructionParser (
        const Namespace& global
    )
    {
        definitionStack.push_back(
            std::make_shared<LambdaFunction>(
                NativeFunction(
                    std::make_shared<NativeFunctionDefinition>(),
                    0, global
                ),
                std::vector<TVarName>{} // global arg names - should be empty
            )
        );
    }

    ConstructionParser& fromCodeStream(
        InCodeStream& inStream
    ) {
        auto topLevel = ParenthesesParser::fromCodeStream(inStream);
        this->prognDef(topLevel);
        return *this;
    }

    NativeFunction finish() {
        if (definitionStack.size() != 1) {
            throw ParseError()
                << __FILE__ << ":" << __LINE__
                << " Definition stack should have size 1";
        }
        return std::move(
            definitionStack.back()->nativeFn
        );
    }

private:
    bool parenthesesExpression(
        ParenthesesParser& pParser
    ) {
        auto nextParser = pParser.nextParser();
        auto token = std::string{};
        if (nextParser.nextToken(token)) {
            if ("defun" == token) {
                functionDef(nextParser);
            } else if ("lambda" == token) {
                lambdaDef(nextParser);
            } else if ("let" == token) {
                letDef(nextParser);
            } else if ("set" == token) {
                setDef(nextParser);
            } else if ("cond" == token) {
                condDef(nextParser);
            } else if ("progn" == token) {
                prognDef(nextParser);
            } else {
                tokenDef(token);
                callDef(nextParser);
            }
        } else {
            if (nextParser.isEnd()) {
                return false;
            }
            if (not parenthesesExpression(nextParser)) {
                return false;
            }
            callDef(nextParser);
        }
        nextParser.close();
        return true;
    }

    bool expression(
        ParenthesesParser& pParser
    ) {
        auto token = std::string{};
        if (pParser.nextToken(token)) {
            tokenDef(token);
        } else {
            // TODO: may be useless
            if (pParser.isEnd()) {
                return false;
            }
            return parenthesesExpression(pParser);
        }
        return true;
    }

    void prognDef(
        ParenthesesParser& pParser
    ) {
        auto core = definitionStack.back()->core();
        while (expression(pParser)) {
            core->addStep(
                NativeFunctionDefinition::LocalStackRewind,
                1
            );
        }
        core->operations.pop_back();
    }

    void functionDef(
        ParenthesesParser& pParser
    ) {
        auto fname = std::string{};
        if (!pParser.nextToken(fname)) {
            throw SyntaxError() << pParser.getStat().toString() << " there is suppose to be function name.";
        }
        lambdaDef(pParser);
        auto core = definitionStack.back()->core();
        auto pos = core->names.size();
        core->names.push_back(fname);
        core->addStep(
            NativeFunctionDefinition::SetLocal,
            pos
        );
    }

    void lambdaDef(
        ParenthesesParser& pParser
    ) {
        auto argNames = ArgNames{};
        auto token = std::string{};
        pParser.check();
        {
            auto argParser = pParser.nextParser();
            while (argParser.nextToken(token)) {
                argNames.push_back(token);
            }
            if (not argParser.isEnd()) {
                throw SyntaxError() << argParser.getStat().toString() << ", code stream of lambda arguments is suppose to be exhausted";
            }
        }
        auto core = definitionStack.back()->core();
        auto argnum = argNames.size();
        auto newLambda = std::make_shared<LambdaFunction>(
            NativeFunction(
                std::make_shared<NativeFunctionDefinition>(
                    std::move(argNames)
                ),
                argnum,
                Namespace{}
            ),
            ArgNames{}
        );
        core->consts.push_back(
            Cell::function(
                newLambda
            )
        );
        core->addStep(
            NativeFunctionDefinition::GetConst,
            core->consts.size() - 1
        );
        definitionStack.push_back(newLambda);
        if (not expression(pParser)) {
            throw SyntaxError() << pParser.getStat().toString() << ", there is suppose to be lambda body";
        }
        pParser.check();
        definitionStack.pop_back();
        argnum = newLambda->names.size();
        core->addStep(
            NativeFunctionDefinition::RunFunction,
            argnum
        );
    }

    void letDef(
        ParenthesesParser& pParser
    ) {
        // TODO
        throw ParseError() << "let construction is not implemented";
    }

    void setDef(
        ParenthesesParser& setParser
    ) {
        auto name = std::string{};
        if (!setParser.nextToken(name)) {
            throw SyntaxError() << setParser.getStat().toString() << ", there is suppose to be name.";
        }
        this->expression(setParser);
        auto core = definitionStack.back()->core();
        auto pos = core->names.size();
        core->names.push_back(name);
        core->addStep(
            NativeFunctionDefinition::SetLocal,
            pos
        );
    }

    void condDef(
        ParenthesesParser& condParser
    ) {
        auto core = definitionStack.back()->core();
        auto endMark = getMarkUid();
        condParser.check();
        while (condParser.isLocked() && not condParser.isEnd()) {
            auto branchParser = condParser.nextParser();
            auto token = std::string{};
            expression(branchParser);
            auto branchEndMark = getMarkUid();
            core->addStep(
                NativeFunctionDefinition::SkipIfNil,
                branchEndMark
            );

            expression(branchParser);
            core->addStep(
                NativeFunctionDefinition::Skip,
                endMark
            );
            core->addStep(
                NativeFunctionDefinition::GuardMark,
                branchEndMark
            );
            branchParser.close();
            condParser.check();
        }
        core->addStep(
            NativeFunctionDefinition::GuardMark,
            endMark
        );
    }

    void callDef(
        ParenthesesParser& pParser
    ) {
        auto argCount = std::size_t{0};
        auto token = std::string{};
        while (expression(pParser)) {
            ++argCount;
        }
        auto core = definitionStack.back()->core();
        core->addStep(
            NativeFunctionDefinition::RunFunction,
            argCount
        );
    }

    void tokenDef(
        const std::string& token
    ) {
        auto cellOpt = tryCellFromString(token);
        if (cellOpt) {
            auto core = definitionStack.back()->core();
            auto pos = core->consts.size();
            core->consts.push_back(
                std::move(cellOpt.get())
            );
            core->addStep(
                NativeFunctionDefinition::GetConst,
                pos
            );
        } else {
            takeVarByName(token);
        }
    }

    void takeVarByName(
        const std::string& varName
    ) {
        auto to = definitionStack.rbegin();
        while (
            to != definitionStack.rend()
            && not to->get()->hasName(varName)
        ) {
            ++to;
        }
        if (to == definitionStack.rend()) {
            throw NameError()
                << "There is no such variable " << Str::Quotes(varName);
        }
        for (
            auto it = definitionStack.rbegin(); it != to; ++it
        ) {
            it->get()->names.push_back(varName);
        }
        for (
            auto it = definitionStack.rbegin(); it <= to; ++it
        ) {
            auto core = it->get()->core();
            auto pos = core->names.size();
            core->names.push_back(varName);
            core->addStep(
                NativeFunctionDefinition::GetLocal,
                pos
            );
        }
    }

    NativeFunctionDefinition::Step::Operand
    getMarkUid() {
        ++markCounter;
        const auto bitSize = 8 * sizeof(NativeFunctionDefinition::Step::Operand);
        return (
            (this->definitionStack.size() << (bitSize / 2)) ^ markCounter
        );
    }

private:
    std::vector<LambdaFunctionPtr> definitionStack;
    NativeFunctionDefinition::Step::Operand markCounter = 0;
};

}  // namespace

NativeFunction parseCode(
    std::istream& in
    , const Namespace& global
) {
    auto codeStream = InCodeStream(in);
    return ConstructionParser(global).fromCodeStream(
        codeStream
    ).finish();
}

}  // namespace AntLisp
