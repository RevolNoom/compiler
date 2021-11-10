/*  
    A logical block of code
    If-then-else, Function, expression (?),...
    OOP Pattern: Composite
*/
#ifndef COMPONENT_HPP 
#define COMPONENT_HPP 

#include <list>
#include <stdexcept>
#include "Token.hpp"

using namespace std;

namespace Component
{
    void Throw(std::string msg, shared_ptr<TokenType::Token> erroneousToken);

    template <typename T>
    shared_ptr<T> TheNextTokenIs(const TokenStream::const_iterator &next)
    {
        return dynamic_pointer_cast<T>(*next);
    }




    class ProgramComponent 
    {
    public:
        void Parse(const TokenStream &ts) 
        {
            Parse(ts.begin(), ts.end());
        }

        virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)  {}/// = 0; 
    protected:
        template <typename T>
        shared_ptr<T> Eat(TokenStream::const_iterator it, std::string errorMsg = std::string()) const
        {
            auto token = TheNextTokenIs<T>(it);
            if (!token)
                Throw(errorMsg, token);
            ++it;
            return token;
        }
    };





    // BLOCK - RELATED 

            class Program: public ProgramComponent 
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

/*
        class ScopeDependent: public ProgramComponent
        {
            void Parse(const TokenStream &ts);
        };

        class Scope: public ScopeDependent 
        {
            void Parse(const TokenStream &ts);
        };

            class Procedure: public Scope
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class Function: public Scope
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class BeginEnd: public Scope
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };


        // Declarations
        class Declaration: public ProgramComponent 
        {
        };

            class DeclarationType: public Declaration
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class DeclarationConst: public Declaration
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class DeclarationVar: public Declaration
            {
            public:
                virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };


    // DATAS

        class Type: public ProgramComponent
        {
        public:
            //virtual void Build(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            virtual void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
        };

   //     class ConcreteType: public ProgramComponent
    class Statement
    {
    public:
        Statement();

        //void Build(TokenStream::const_iterator begin, TokenStream::const_iterator end);
        void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end);

    //    protected:
    //        const shared_ptr<Component>& LookupIdent(shared_ptr<Identifier> id);
    };
    class DeclarationFunction
    class BeginEnd

    class Assignment
    class CallProc
    class IfElse
    class While
    class For

    class TypeBasic
    class ParamList

    class Condition
    class Expression
    class Factor: public ProgramComponent
    {
    public:
        void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end);
    
    private:

        shared_ptr<Component> _Factor
    }
    */
}

#endif