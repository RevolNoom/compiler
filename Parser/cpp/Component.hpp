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





        // THOSE WHO LIVES BY THE LAW

        class ScopeDependent: public ProgramComponent
        {
        public:
        };

            // THOSE WHO MAKES THE LAW 

            class Scope: public ScopeDependent 
            {
            public:
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
                
                	
                };


                class Function: public Scope
                {
                public:
                	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                
                	
                };


        // DECLARATIONS

        class Declaration: public ProgramComponent 
        {
        public:
        	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
        
        	
        };

            class DeclareType: public Declaration
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            };

            class DeclareConst: public Declaration
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            };

            class DeclareVar: public Declaration
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            };

            class DeclareParam: public Declaration 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            };


        // STATEMENTS

        // Wrapper class
        class Statement: public ScopeDependent
        {
        public:
        	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
        
        	
        };

        // Statement's Wrapped classes
        class ConcreteStatement: public ScopeDependent 
        {
        public:
        };

            class BeginEnd: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            };

            class While: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };

            class For: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };

            class Call: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };

            class If: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };
            
            class Assignment: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };

            class EmptyStatement: public ConcreteStatement 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            
                
                
            };


    // DATAS

        // Base class for all types
        class Type: public ScopeDependent 
        {
        public:
            //bool IsType(shared_ptr<Type> anotherType);
        };

            // Add Qualifier to types
            // This is the class you use when you create a type
            class FullType: public Type 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            protected:
                bool _const;
                shared_ptr<Component::Type> _type;
            };



            // Not fully implemented yet
            // COMPOSITE
            class Array: public Type 
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            };

            class BasicType: public Type
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
                enum 
                {
                    CHAR=0,
                    INTEGER=1
                };

            private:
                bool _type;
            };

            class Alias: public Type
            {
            public:
            	virtual TokenStream::const_iterator Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end) override;
            
            	
            private:
                shared_ptr<TokenType::Identifier> _alias;
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
}

#endif