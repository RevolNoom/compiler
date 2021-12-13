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
#include <array>
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

        // TODO: Add Argument Semantically 
        void ParseStream(const TokenStream &ts) 
        {
            Parse(ts.begin(), ts.end());
        }

        // Override this
        virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
        {
            // Doin Nothin
            return begin;
        }
        
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

        // Those who lives in a scope 

        class ScopeDependent: virtual public ProgramComponent
        {
        protected:
            // Lookup a Program Component with Identifier id, return it
            // nullptr if it does not exist
            virtual shared_ptr<ProgramComponent> Lookup(int DeclarationType, shared_ptr<TokenType::Identifier> id);

            shared_ptr<ScopeDependent> _outerScope;
        }; 

            // STATEMENTS

            class Statement: public ScopeDependent
            {
            public:
                static shared_ptr<Statement> Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) = 0;
            };

                class BeginEnd: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                private:
                    list<shared_ptr<Component::Statement>> _statements;
                };

                class While: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };

                class For: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };

                class Call: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };

                class If: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };
                
                class Assignment: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };

                class EmptyStatement: public Statement 
                {
                public:
                    virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                };


        // DATAS

            // Base class for all types
            class Type: virtual public ProgramComponent 
            {
            public:
                // Debugging purpose
                virtual string GetTypeName() = 0;

                static shared_ptr<Component::Type> Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                virtual bool IsType(shared_ptr<Component::Type> otherType) = 0;
                virtual shared_ptr<Component::Type> Clone() = 0;

                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
                {
                    Throw("Compiler Bug: Must decide to parse value or parse type", *begin);
                    return begin;
                }

                virtual TokenStream::const_iterator ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end) = 0;
                virtual TokenStream::const_iterator ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end) = 0;
            };

                class Array: public Type 
                {
                public:
                    //virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    shared_ptr<Component::Type> GetElementType() {return _elementType;}
                    virtual bool IsType(shared_ptr<Component::Type> otherType) override;
                    virtual shared_ptr<Component::Type> Clone() override;
                    virtual TokenStream::const_iterator ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    virtual TokenStream::const_iterator ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;

                    // Debugging purpose
                    virtual string GetTypeName() override 
                    { 
                        return string("Array of ") + _elementType->GetTypeName();
                    }

                private:
                    int _size;
                    shared_ptr<Component::Type> _elementType;
                };

                // This class serves both as a type checker and a data holder
                // When used as a type checker, its value is irrelevant
                class BasicType: public Type
                {
                public:
                    static shared_ptr<Component::BasicType> Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                    static shared_ptr<Component::BasicType> MakeChar(char value);
                    static shared_ptr<Component::BasicType> MakeInteger(int value);

                    //virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    virtual shared_ptr<Component::Type> Clone() override;
                    virtual bool IsType(shared_ptr<Component::Type> otherType) override;
                    virtual TokenStream::const_iterator ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    virtual TokenStream::const_iterator ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;

                    enum TypeTag
                    {
                        UNKNOWN,
                        CHAR,
                        INTEGER
                    }; 

                    union ConstantValue
                    {
                        char _c;
                        int _i;
                    };

                    ConstantValue GetValue() {return _value;}
                    // Debugging purpose
                    virtual string ToString()
                    {
                        string result;
                        if (_typeTag == INTEGER)
                        {
                            result = to_string(_value._i);
                        }
                        else
                        {
                            result = "a";
                            result[0] = _value._c;
                        }
                        return result;  
                    }

                    // Debugging purpose
                    virtual string GetTypeName() override 
                    { 
                        // Omit Unknown case
                        return _typeTag == CHAR? "Char" : "Integer"; 
                    }

                private:
                    int _typeTag; 
                    ConstantValue _value;
                };

            // EXPRESSION AND ITS COMPONENTS

            class Term: public ScopeDependent
            {
            public:
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class Factor: public ScopeDependent
            {
            public:
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class Expression: public ScopeDependent
            {
            public:
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };


            class Condition: public ScopeDependent
            {
            public:
                virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            };




            // And of course, SCOPES

            class Scope: public ScopeDependent 
            {
            public:
                enum
                {
                    DECL_CONST,
                    DECL_TYPE,
                    DECL_VAR,
                    DECL_PROC,
                    DECL_FUNC,
                    DECL_MAX 
                };

                shared_ptr<TokenType::Identifier> GetIdentifier() {return _id;}
                void PrintSymbolTable(int tab = 0);

            protected:

                virtual shared_ptr<ProgramComponent> Lookup(int DeclarationType, shared_ptr<TokenType::Identifier> id) override;

                TokenStream::const_iterator RegisterDeclarations(TokenStream::const_iterator begin, TokenStream::const_iterator end);

                // Perform check before adding symbol to the table
                // If the symbol does not exist in this scope yet, it's added and return true
                // False otherwise
                void RegisterSymbol(int DeclarationType, shared_ptr<TokenType::Identifier> symbol, shared_ptr<ProgramComponent> content);

                // Name of this scope
                shared_ptr<TokenType::Identifier> _id;

            private:
                TokenStream::const_iterator RegisterConst(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                TokenStream::const_iterator RegisterType(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                TokenStream::const_iterator RegisterVar(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                TokenStream::const_iterator RegisterProcedure(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                TokenStream::const_iterator RegisterFunction(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                TokenStream::const_iterator RegisterBeginEnd(TokenStream::const_iterator begin, TokenStream::const_iterator end);

                // Symbol Table: 
                // +) An array of DECL_MAX lists ( = 5 lists)
                // +) Each list contains ProgramComponents tagged with a name
                array<  list<pair<shared_ptr<TokenType::Identifier>, 
                            shared_ptr<ProgramComponent>>> ,
                        DECL_MAX> _symbolTable;

                // The code body of this scope
                shared_ptr<BeginEnd> _body;
            };


                class Global: public Scope 
                {
                public:
                	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                };

                class Program: public Scope 
                {
                public:
                	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                	
                };

                class Procedure: public Scope
                {
                public:
                	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                	TokenStream::const_iterator ParseParamList(TokenStream::const_iterator begin, TokenStream::const_iterator end);
                    TokenStream::const_iterator ParseArguments(TokenStream::const_iterator begin, TokenStream::const_iterator end);

                private:
                    list<shared_ptr<Component::BasicType>> _paramTypes;
                };


                class Function: public Procedure, public Type 
                {
                public:
                    using ProgramComponent::Eat;

                    using ProgramComponent::EatIfPossible;

                	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    virtual bool IsType(shared_ptr<Component::Type> otherType) override;
                    shared_ptr<Component::BasicType> GetReturnType();

                    string GetTypeName() override;
                    virtual shared_ptr<Component::Type> Clone() override;
                    virtual TokenStream::const_iterator ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                    virtual TokenStream::const_iterator ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;

                private:
                    shared_ptr<Component::BasicType> _returnType;
                };
}

#endif