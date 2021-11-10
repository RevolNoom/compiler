/*
    SINGLETON + VISITOR

    Parse a token stream into a Component(?)
*/
#ifndef PARSER_HPP
#define PARSER_HPP

#include "Token.hpp"
#include "Component.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;
using namespace TokenType;

namespace ParserType
{
    using namespace ProgramComponent;

    class Parser
    {
    public:
        // Parse a Token stream
        // Say goodbye to your stream because you May NOT see it ever again
        // Who knows? If you are lucky... :D
        virtual void Parse(TokenStream &ts) const = 0;


    protected:
        virtual void Throw(std::string msg, const shared_ptr<Token> troubleToken = nullptr) const
        {
            if (troubleToken)
                msg = string("On line ") + to_string(troubleToken->GetLine()) + 
                        string(", col ") + to_string(troubleToken->GetCol()) + 
                        string(", at token ") + troubleToken->GetTokenType() + 
                        string(":\n") + msg;
                
            throw std::invalid_argument(msg);
        }
    };


    class TopDown: public Parser
    {
    public:
        static Parser* Singleton();

        virtual void Parse(TokenStream &ts) const;

    protected:
        template<typename T>
        shared_ptr<T> TheNextTokenIs(const TokenStream &ts) const
        {
            return ts.size()? dynamic_pointer_cast<T>(ts.front()) : nullptr;
        }

        template<typename T>
        void Eat(TokenStream &ts, string errorMsg) const
        {
            if (!TheNextTokenIs<T>(ts))
                Throw(errorMsg, ts.front());
            std::cout<<" " + ts.front()->GetTokenType() + " ";
            ts.pop_front();
        }

        virtual void ParseProgram(TokenStream &ts) const;
        virtual void ParseBlock(TokenStream &ts) const;
        virtual void ParseStatement(TokenStream &ts) const;

        virtual void ParseDeclConst(TokenStream &ts) const;
        virtual void ParseDeclType(TokenStream &ts) const;
        virtual void ParseDeclVar(TokenStream &ts) const;
        virtual void ParseDeclProcedure(TokenStream &ts) const;
        virtual void ParseDeclFunction(TokenStream &ts) const;
        virtual void ParseBeginEnd(TokenStream &ts)const;

        virtual void ParseAssignment(TokenStream &ts) const;
        virtual void ParseCallProc(TokenStream &ts) const;
        virtual void ParseIfElse(TokenStream &ts) const;
        virtual void ParseWhile(TokenStream &ts) const;
        virtual void ParseFor(TokenStream &ts) const;

        virtual void ParseType(TokenStream &ts) const;
        virtual void ParseTypeBasic(TokenStream &ts) const;
        virtual void ParseParamList(TokenStream &ts) const;

        virtual void ParseCondition(TokenStream &ts) const;
        virtual void ParseExpression(TokenStream &ts) const;
        virtual void ParseFactor(TokenStream &ts) const;
        virtual void ParseTerm(TokenStream &ts) const;



    private:
        TopDown();
        static TopDown* _singleton;
    };
}

#endif
