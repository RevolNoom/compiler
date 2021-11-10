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

void Component::Program::Build(TokenStream &ts)
{
    std::cout<<"Parsing Program\n";
    Eat<TokenType::Program>(ts, "Missing keyword Program.");

    this->_id = Eat<TokenType::Identifier>(ts, "Missing Program Identifier");

    Eat<TokenType::Semicolon>(ts, "Missing Semicolon.");
    
    // TODO:: BLOCK

    Eat<TokenType::Period>(ts, "Missing Period to end Program.");
    std::cout<<"Parsing Program Completed\n";
}


void Component::DeclarationType::Build(TokenStream &ts)
{
    std::cout<<"Parsing Type Declaration.\n";

    Eat<TokenType::Type>(ts, "Type declaration does not start with \"Type\" but " + ts.front()->GetTokenType());

    while (1)
    {
        auto ID = Eat<Identifier>(ts, "Missing Identifier for Type declaration.");

        Eat<Assign>(ts, "Missing assign symbol.");

        // TODO: Parse Type then add to declaredID
        //ParseType(ts);

        Eat<Semicolon>(ts, "Missing semicolon.");

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }

    std::cout<<"Parsing Type Declaration Completed\n";
}

void Component::DeclarationConst::Build(TokenStream &ts)
{
    std::cout<<"Parsing Type Declaration.\n";

    Eat<TokenType::Const>(ts, "Const declaration does not start with \"Const\" but " + ts.front()->GetTokenType());

    // Gobble up all const assignments 
    while (1)
    {
        auto ID = Eat<TokenType::Identifier>(ts, "Missing Identifier for constant declaration.");

        Eat<TokenType::Assign>(ts, "Missing assign symbol.");

        auto cnst = make_shared<Constant>();
        cnst->Parse(ts);

        _declaredId.push_back({ID, cnst});

        // Semicolon ';'
        Eat<TokenType::Semicolon>(ts, "Missing semicolon."); 

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }

    std::cout<<"Parsing Const Declaration Completed\n";
}

void Component::DeclarationVar::Build(TokenStream &ts)
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

	std::cout<<"\nParse Var Declaration Completed.\n";
}


void Component::Constant::Build(TokenStream &ts)
{
    /*
        + A Letter
        or
        + Plus/Minus symbol followed by ConstantIdentifier/UnsignedInt
    */
    if (TheNextTokenIs<Letter>(ts))
        _data = Eat<TokenType::Token>(ts, "");
    else
    {
        if (TheNextTokenIs<Plus>(ts) ||
            TheNextTokenIs<Minus>(ts))
        // TODO: Also account this sign
            Eat<TokenType::Token>(ts, "");

        // TODO: Add Const Identifier 
        _data = Eat<TokenType::Number>(ts, string("Not a number constant. It's a ") + ts.front()->GetTokenType());
    }
}