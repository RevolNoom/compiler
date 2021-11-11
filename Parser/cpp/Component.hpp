/*  
    A logical block of code
    If-then-else, Function, expression (?),...
    OOP Pattern: Composite
*/
#ifndef COMPONENT_HPP 
#define COMPONENT_HPP 

#include <list>
#include <stdexcept>
#include <iostream>
#include "Token.hpp"

using namespace std;

namespace Component
{
    void Throw(std::string msg, shared_ptr<TokenType::Token> erroneousToken);

    template <typename T>
    shared_ptr<T> ThisTokenIs(const TokenStream::const_iterator &next)
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

        virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const = 0; 
    protected:

        // Check if Iterator contains shared_ptr type T, and then move it forward
        template <typename T>
        shared_ptr<T> Eat(TokenStream::const_iterator &it, std::string errorMsg) const
        {
            auto token = EatIfPossible<T>(it);
            if (!token)
                Throw(errorMsg, *it);
            return token;
        }

        // If the next token is type T, then eat
        // Lost interest if it's not
        template <typename T>
        shared_ptr<T> EatIfPossible(TokenStream::const_iterator &it) const
        {
            auto token = ThisTokenIs<T>(it);
            if (token)
            {
                std::cout<<">>"<<(*it)->GetTokenType()<<"\n";
                ++it;
            }
            return token;
        }
    };





    // 



        // THOSE WHO LIVES BY THE LAW

        class ScopeDependent: public ProgramComponent
        {
        };

            // THOSE WHO MAKES THE LAW 

            class Scope: public ScopeDependent 
            {
            };

                class Program: public Scope 
                {
                public:
                    // 2-hours debugging, and the vtable breaks because this line was missing
                    using ProgramComponent::Parse;
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
                };

                class Procedure: public Scope
                {
                public:
                    using ProgramComponent::Parse;
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
                };


                class Function: public Scope
                {
                public:
                    using ProgramComponent::Parse;
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
                };


        // DECLARATIONS

        class Declaration: public ProgramComponent 
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

            class DeclareType: public Declaration
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class DeclareConst: public Declaration
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class DeclareVar: public Declaration
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class DeclareParam: public Declaration 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;

            };


        // STATEMENTS

        // Wrapper class
        class Statement: public ScopeDependent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

        // Statement's Wrapped classes
        class ConcreteStatement: public ProgramComponent
        {
        };

            class BeginEnd: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class While: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class For: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class Call: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

            class If: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };
            
            class Assignment: public ConcreteStatement 
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

    // DATAS

        // Wrapper Class
        class Type: public ProgramComponent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

            // Wrapped Classes

            // Not currently needed 
            // class Char
            // class Integer

            // Not fully implemented yet
            // COMPOSITE
            class Array: public ProgramComponent
            {
            public:
                using ProgramComponent::Parse;
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
            };

        // EXPRESSION AND ITS MINIONS

        class Term: public ScopeDependent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

        class Factor: public ScopeDependent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

        class Expression: public ScopeDependent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };

        // Not really a minion. Actually a client
        class Condition: public ScopeDependent
        {
        public:
            using ProgramComponent::Parse;
            virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const override;
        };
/*
    class Condition
    class Expression
    class Factor: public ProgramComponent
    {
    public:
        void Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) const;
    
    private:

        shared_ptr<Component> _Factor
    }
    */
}

#endif