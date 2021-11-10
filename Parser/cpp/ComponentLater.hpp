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

    class ProgramComponent 
    {
    public:
        void Parse(TokenStream &ts) {}//= 0;
        //void Build(TokenStream &ts) {}//= 0;
        //virtual vector<string> ToMachineCode() = 0; {return {};};//

    protected:

        template <typename T>
        shared_ptr<T> TheNextTokenIs(TokenStream &ts) const
        {
            return ts.size()? dynamic_pointer_cast<T>(ts.front()) : nullptr;
        }

        template <typename T>
        shared_ptr<T> Eat(TokenStream &ts, std::string errorMsg = std::string()) const
        {
            auto token = TheNextTokenIs<T>(ts);
            if (!token)
                Throw(errorMsg, token);
            ts.pop_front();
            return token;
        }

    };


    // BLOCK - RELATED 

        class ScopeDependent: public ProgramComponent
        {

        };

        // CHAIN OF RESPONSIBILITY
        // When a block needs to lookup an identifier,
        // It browses through the ids it knows first
        // If the id's not found, pass up the job to its parent
        class Scope: public ScopeDependent 
        {
        protected:
        /*
            virtual const shared_ptr<ProgramComponent>& LookupIdent(shared_ptr<Identifier> id) 
            {
                // TODO
                return shared_ptr<ProgramComponent>(this);
            }

        protected:
            // Who's storing this block?
            shared_ptr<ProgramComponent> _parentBlock;

            // Identifier of this block
            shared_ptr<TokenType::Identifier> _id;

            list<pair<shared_ptr<TokenType::Identifier>, shared_ptr<ProgramComponent>>> _declaredId;
        */
        };

        // Block Sub - Classes

            class Program: public Scope 
            {
            public:
                //void Build(TokenStream &ts);
                void Parse(TokenStream &ts);
            };

            class Procedure: public Scope
            {
            public:
                //void Build(TokenStream &ts);
                void Parse(TokenStream &ts);
            };

            class Function: public Scope
            {
            public:
                //void Build(TokenStream &ts);
                void Parse(TokenStream &ts);
            };

            class BeginEnd: public Scope
            {
            public:
                //void Build(TokenStream &ts);
                void Parse(TokenStream &ts);
            };

                // Declarations
                class Declaration: public ProgramComponent 
                {
                public:
                    // TODO: shared_ptr<ProgramComponent> Lookup(shared_ptr<TokenType::Identifier>)

                    void Lookup(shared_ptr<TokenType::Identifier>) const = 0;

                protected:
                    // list
                }

                class DeclarationType: public Declaration
                {
                public:
                    //void Build(TokenStream &ts);
                    void Parse(TokenStream &ts);
                };

                class DeclarationConst: public Declaration
                {
                public:
                    //void Build(TokenStream &ts);
                    void Parse(TokenStream &ts);
                };

                class DeclarationVar: public Declaration
                {
                public:
                    //void Build(TokenStream &ts);
                    void Parse(TokenStream &ts);
                };
        

    // DATAS

        class Constant: public ProgramComponent
        {
        public:
            //void Build(TokenStream &ts);
            void Parse(TokenStream &ts);
        
        private:
            shared_ptr<TokenType::Token> _data;
        };


    
        class Type: public ProgramComponent
        {
        public:
            //void Build(TokenStream &ts);
            void Parse(TokenStream &ts);
        
        private:
            shared_ptr<ConcreteType> _type;
        };

   //     class ConcreteType: public ProgramComponent
/*
    class Statement
    {
    public:
        Statement();

        //void Build(TokenStream &ts);
        void Parse(TokenStream &ts);

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
        void Parse(TokenStream &ts);
    
    private:

        shared_ptr<Component> _Factor
    }
    */
}

#endif