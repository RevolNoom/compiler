#include "Component.hpp"
#include <iostream>

using namespace TokenType;

void Component::Throw(std::string msg, shared_ptr<TokenType::Token> erroneousToken=nullptr)
{
    if (erroneousToken)
        msg = string("On line ") + to_string(erroneousToken->GetLine()) + 
                string(", col ") + to_string(erroneousToken->GetCol()) + 
                string(", at token ") + erroneousToken->GetTokenType() + 
                string(":\n") + msg;

    throw std::invalid_argument(msg);
}

//void Component::Program::Parse(const TokenStream &ts){}
void Component::Program::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"\nParsing Program\n";

    // Syntax:
    // PROGRAM IDENTIFIER: BLOCK .

    Eat<TokenType::Program>(begin, "Program does not start with keyword Program");

    Eat<TokenType::Identifier>(begin, "Identifier expected for program name. \'" + (*begin)->GetTokenType() + "\' does not look like a valid Identifier.");
        
    Eat<TokenType::Semicolon>(begin, "Missing Semicolon ';' after program name.");

    //ParseBlock(ts);
    
    Eat<TokenType::Period>(begin, "Your program misses a period '.' in the end.");
	std::cout<<"\nProgram Parsed\n";
}

/*

void Component::DeclarationType::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Types Declaration.\n";

    Eat<TokenType::Type>(ts, "Type declaration does not start with \"Type\" but " + ts.front()->GetTokenType());

    while (1)
    {
        Eat<Identifier>(ts, "Missing Identifier for Type declaration.");

        Eat<Assign>(ts, "Missing assign symbol.");

        //ParseType(ts);

        Eat<Semicolon>(ts, "Missing semicolon.");

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }

    std::cout<<"Type Declaration Parsed\n";
}

void Component::DeclarationConst::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Type Declaration.\n";

    Eat<TokenType::Const>(ts, "Const declaration does not start with \"Const\" but " + ts.front()->GetTokenType());

    // Gobble up all const assignments 
    while (1)
    {
        Eat<TokenType::Identifier>(ts, "Missing Identifier for constant declaration.");

        Eat<TokenType::Assign>(ts, "Missing assign symbol.");

        auto cnst = make_shared<Constant>();
        cnst->Parse(ts);

        // Semicolon ';'
        Eat<TokenType::Semicolon>(ts, "Missing semicolon."); 

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }

    std::cout<<"Const Declaration Parsed\n";
}

void Component::DeclarationVar::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"\nParse Var Declaration.\n";

    Eat<Var>(ts, "Why did VAR declaration started with " + ts.front()->GetTokenType() + "?");

    while (1)
    {
        // TODO: Add var to declaredID
        auto ID = Eat<Identifier>(ts, "Missing Identifier for VAR declaration.");

        Eat<Colon>(ts, "Missing colon \':\'.");

        ParseType(ts);

        Eat<Semicolon>(ts, "Missing semicolon."); 

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }

	std::cout<<"\nVar Declaration Parsed.\n";
}


void Component::Constant::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)

    // A Letter
    // or
    // Plus/Minus symbol followed by ConstantIdentifier/UnsignedInt

    if (TheNextTokenIs<Letter>(ts))
        Eat<TokenType::Token>(ts, "");
    else
    {
        if (TheNextTokenIs<Plus>(ts) ||
            TheNextTokenIs<Minus>(ts))
        // TODO: Also account this sign
            Eat<TokenType::Token>(ts, "");

        // TODO: Add Const Identifier 
        Eat<TokenType::Number>(ts, string("Not a number constant. It's a ") + ts.front()->GetTokenType());
    }
}*/