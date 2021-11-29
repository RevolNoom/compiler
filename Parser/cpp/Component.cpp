#include "Component.hpp"
#include <iostream>

using namespace Component;

void Component::Throw(std::string msg, shared_ptr<TokenType::Token> erroneousToken=nullptr)
{
    if (erroneousToken)
        msg = string("On line ") + to_string(erroneousToken->GetLine()) + 
                string(", col ") + to_string(erroneousToken->GetCol()) + 
                string(", at token ") + erroneousToken->GetTokenType() + 
                string(":\n") + msg;

    throw std::invalid_argument(msg);
}

TokenStream::const_iterator Component::Program::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parsing Program\n";

    Eat<TokenType::Program>(begin, "Program does not start with keyword Program");

    Eat<TokenType::Identifier>(begin, "Identifier expected for program name. \'" + (*begin)->GetTokenType() + "\' does not look like a valid Identifier.");
        
    Eat<TokenType::Semicolon>(begin, "Missing Semicolon ';' after program name.");

    auto declarations = make_shared<Declaration>();
    begin = declarations->Parse(begin, end);
    
    Eat<TokenType::Period>(begin, "Your program misses a period '.' at the end.");

    if (begin != end)
        Throw("Trailing characters after Program. Please erase them away.", *begin);
    else
	    std::cout<<"Program Parsed\n";

    return begin;
}

TokenStream::const_iterator Component::Declaration::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Declarations (Block)\n";

    std::list<shared_ptr<Component::ProgramComponent>> declarations
    {
        make_shared<DeclareConst>(),
        make_shared<DeclareType>(),
        make_shared<DeclareVar>(),
        make_shared<Procedure>(),
        make_shared<Function>(),
        make_shared<BeginEnd>()
    };

    for (const auto &decl: declarations)
        begin = decl->Parse(begin, end);

    std::cout<<"Declarations (Block) Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::DeclareConst::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Const Declaration\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Const>(begin))
    {
        // Gobble up all const assignments 
        while (1)
        {
            Eat<TokenType::Identifier>(begin, "Missing Identifier for constant declaration.");

            Eat<TokenType::Assign>(begin, "Missing assign symbol.");

            // Get the constant value
            if (!EatIfPossible<TokenType::Letter>(begin))
            {
                // Eat + or -. Only ONE of them
                (EatIfPossible<TokenType::Plus>(begin) || EatIfPossible<TokenType::Minus>(begin)); 

                if (! (EatIfPossible<TokenType::Identifier>(begin) ||
                       EatIfPossible<TokenType::Number>(begin)     || 
                       EatIfPossible<TokenType::Letter>(begin)))
                        Throw("Invalid Constant.", *begin);
            }
            
            Eat<TokenType::Semicolon>(begin, "Missing semicolon."); 

            // Loop if the next Token is an Identifier.
            if (!ThisTokenIs<TokenType::Identifier>(begin))
                break;
        }
    }

    std::cout<<"Const Declaration Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::DeclareType::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Types Declaration.\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Type>(begin))
    {
        while (1)
        {
            Eat<TokenType::Identifier>(begin, "Missing Identifier for Type declaration.");

            Eat<TokenType::Assign>(begin, "Missing assign symbol.");

            auto type = make_shared<FullType>();
            begin = type->Parse(begin, end);

            Eat<TokenType::Semicolon>(begin, "Missing semicolon.");

            // Loop if the next Token is an Identifier.
            if (!ThisTokenIs<TokenType::Identifier>(begin))
                break;
        }
    }
    std::cout<<"Type Declaration Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::DeclareVar::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Var Declaration.\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Var>(begin))
    {
        while (1)
        {
            Eat<TokenType::Identifier>(begin, "Missing Identifier for VAR declaration.");

            Eat<TokenType::Colon>(begin, "Missing colon \':\'.");

            auto type = make_shared<FullType>();
            begin = type->Parse(begin, end);

            Eat<TokenType::Semicolon>(begin, "Missing semicolon."); 

            // Loop if the next Token is an Identifier.
            if (!ThisTokenIs<TokenType::Identifier>(begin))
                break;
        }
    }

	std::cout<<"Var Declaration Parsed.\n";
    return begin;
}

TokenStream::const_iterator Component::Procedure::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Procedure Declaration\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Procedure>(begin))
    {
        Eat<TokenType::Identifier>(begin, "Missing procedure Identifier.");

        // A ParamList is optional 
        if (ThisTokenIs<TokenType::ParenthesisOpen>(begin))
        {
            auto declParam = make_shared<DeclareParam>();
            begin = declParam->Parse(begin, end);
        }

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon.");

        auto declarations = make_shared<Declaration>();
        begin = declarations->Parse(begin, end);

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon at the procedure declaration end.");
    }

	std::cout<<"Procedure Declaration Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::Function::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Function Declaration\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Function>(begin))
    {
        Eat<TokenType::Identifier>(begin, "Missing Function Identifier.");

        auto declParam = make_shared<DeclareParam>();
        begin = declParam->Parse(begin, end);

        Eat<TokenType::Colon>(begin, "Missing Colon ':' before return type.");

        auto bstype = make_shared<BasicType>();
        begin = bstype->Parse(begin, end);

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon.");

        auto declarations = make_shared<Declaration>();
        begin = declarations->Parse(begin, end);

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon at the function declaration end.");
    }

	std::cout<<"Function Declaration Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::BeginEnd::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Begin-End\n";

    Eat<TokenType::Begin>(begin, "Missing Begin-End block.");

    auto stm = make_shared<Statement>();
    begin = stm->Parse(begin, end);

    while (EatIfPossible<TokenType::Semicolon>(begin))
    {
        begin = stm->Parse(begin, end);
    }

    Eat<TokenType::End>(begin, "Missing End keyword.");
	std::cout<<"Begin-End Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::FullType::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing FullType\n";

    // Try pinning which type is this
    shared_ptr<Type> type = make_shared<BasicType>();
    if (ThisTokenIs<TokenType::Identifier>(begin))
        type = make_shared<Alias>();
    else if (ThisTokenIs<TokenType::Array>(begin))
        type = make_shared<Component::Array>();
        
    begin = type->Parse(begin, end);

    std::cout<<"FullType Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::Array::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Array\n";

    Eat<TokenType::Array>(begin, "Unknown type.");
    Eat<TokenType::ArrayIndexOpen>(begin, "Missing array index symbol '(.'");
    Eat<TokenType::Number>(begin, "Array size not specified");
    Eat<TokenType::ArrayIndexClose>(begin, "Missing array index symbol '.)'");
    Eat<TokenType::Of>(begin, "Missing keyword Of");

    auto type = make_shared<FullType>();
    begin = type->Parse(begin, end);

    std::cout<<"Array Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::DeclareParam::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing ParamList\n";

    // Param Declaration is optional
    if (EatIfPossible<TokenType::ParenthesisOpen>(begin))
    {
        while (1)
        {
            EatIfPossible<TokenType::Var>(begin);
            Eat<TokenType::Identifier>(begin, "Missing Identifier.");
            Eat<TokenType::Colon>(begin, "Missing colon ':'.");
            
            auto bstype = make_shared<BasicType>();
            begin = bstype->Parse(begin, end);

            // Closing parenthesis means no more parameters
            if (!EatIfPossible<TokenType::Semicolon>(begin))
                break;
        }
    }
    
    Eat<TokenType::ParenthesisClose>(begin, "Missing closing parenthesis ')' on parameter list.");

    std::cout<<"ParamList Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::If::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing If\n";

    if (EatIfPossible<TokenType::If>(begin))
    {
        auto cdt = make_shared<Condition>();
        begin = cdt->Parse(begin, end);

        Eat<TokenType::Then>(begin, "There must be keyword 'Then' to preceding statements");

        auto stm = make_shared<Statement>();
        begin = stm->Parse(begin, end);

        if (EatIfPossible<TokenType::Else>(begin))
        {
            auto stm = make_shared<Statement>();
            begin = stm->Parse(begin, end);
        }
    }

    std::cout<<"If Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::While::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing While\n";

    if (EatIfPossible<TokenType::While>(begin))
    {
        auto cdt = make_shared<Condition>();
        begin = cdt->Parse(begin, end);

        Eat<TokenType::Do>(begin, "There must be keyword 'Do' preceding statements");

        auto stm = make_shared<Statement>();
        begin = stm->Parse(begin, end);
    }

    std::cout<<"While Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::For::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing For\n";

    if (EatIfPossible<TokenType::For>(begin))
    {
        auto asgm = make_shared<Assignment>();
        begin = asgm->Parse(begin, end);

        Eat<TokenType::To>(begin, "Did you forget 'To' keyword? You need it to specify a range.");

        auto exp = make_shared<Expression>();
        begin = exp->Parse(begin, end);

        Eat<TokenType::Do>(begin, "Keyword 'Do' is needed after range declaration.");

        auto stm = make_shared<Statement>();
        begin = stm->Parse(begin, end);
    }

    std::cout<<"For Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Call::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Call\n";

    if (EatIfPossible<TokenType::Call>(begin))
    {
        Eat<TokenType::Identifier>(begin, "Missing procedure identifier.");

        if (EatIfPossible<TokenType::ParenthesisOpen>(begin))
        {
            // Parsing Arguments
            auto exp = make_shared<Expression>();
            begin = exp->Parse(begin, end);
            while (EatIfPossible<TokenType::Comma>(begin))
                begin = exp->Parse(begin, end);

            Eat<TokenType::ParenthesisClose>(begin, "Missing closing parenthesis.");
        }
    }

    std::cout<<"Call Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Assignment::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Assignment\n";

    if (EatIfPossible<TokenType::Identifier>(begin))
    {
        Eat<TokenType::Assign>(begin, "Missing assignment symbol ':='.");

        auto exp = make_shared<Expression>();

        begin = exp->Parse(begin, end);
    }


    std::cout<<"Assignment Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::EmptyStatement::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Empty Statement\nEmpty Statement Parsed\n";
    return begin;
}



TokenStream::const_iterator Component::Factor::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Factor\n";
    
    auto exp = make_shared<Expression>();

    if (EatIfPossible<TokenType::Identifier>(begin))
    {
        if (EatIfPossible<TokenType::ParenthesisOpen>(begin))
        {
            begin = exp->Parse(begin, end);
            while (EatIfPossible<TokenType::Comma>(begin))
                begin = exp->Parse(begin, end);
            Eat<TokenType::ParenthesisClose>(begin, "Missing closing parenthesis.");
        }
        else if (EatIfPossible<TokenType::ArrayIndexOpen>(begin))
        {
            begin = exp->Parse(begin, end);
            Eat<TokenType::ArrayIndexClose>(begin, "Missing Array index closing symbol '.)'");
        }
    }
    else if (EatIfPossible<TokenType::ParenthesisOpen>(begin))
    {
        begin = exp->Parse(begin, end);
        Eat<TokenType::ParenthesisClose>(begin, "Missing closing parenthesis.");
    }
    else if (! (EatIfPossible<TokenType::Letter>(begin) ||
                EatIfPossible<TokenType::Number>(begin)))
                {
                    Throw("Can't parse this token into a factor.", *begin);
                }

    std::cout<<"Factor Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Term::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Term\n";
    auto factor = make_shared<Factor>();
    begin = factor->Parse(begin, end);

    while ( EatIfPossible<TokenType::Times>(begin) ||
            EatIfPossible<TokenType::Slash>(begin))
                begin = factor->Parse(begin, end);

    std::cout<<"Term Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Expression::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Expression\n";

    // Let's see if a bug appear or not
    // Eat if present: + or -
    // But only eat ONE thing. If two things are eaten, fix this
    (EatIfPossible<TokenType::Plus>(begin) || EatIfPossible<TokenType::Plus>(begin));

    auto term = make_shared<Term>();
    begin = term->Parse(begin, end);

    while (EatIfPossible<TokenType::Minus>(begin) ||
            EatIfPossible<TokenType::Plus>(begin))
            begin = term->Parse(begin, end);

    std::cout<<"Expression Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Statement::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Statement\n";
    
    shared_ptr<ProgramComponent> concreteStatement = make_shared<EmptyStatement>();

    if (ThisTokenIs<TokenType::If>(begin))
        concreteStatement = make_shared<If>();
    else if (ThisTokenIs<TokenType::Call>(begin))
        concreteStatement = make_shared<Call>();
    else if (ThisTokenIs<TokenType::For>(begin))
        concreteStatement = make_shared<For>();
    else if (ThisTokenIs<TokenType::While>(begin))
        concreteStatement = make_shared<While>();
    else if (ThisTokenIs<TokenType::Identifier>(begin))
        concreteStatement = make_shared<Assignment>();
    else if (ThisTokenIs<TokenType::Begin>(begin))
        concreteStatement = make_shared<BeginEnd>();

    begin = concreteStatement->Parse(begin, end);

    std::cout<<"Statement Parsed\n";
    return begin;
}


// T Y P E S
TokenStream::const_iterator Component::BasicType::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    if (! (EatIfPossible<TokenType::Char>(begin) ||
            EatIfPossible<TokenType::Integer>(begin)))
        Throw("Not a basic type", *begin);
    return begin;
}

TokenStream::const_iterator Component::Alias::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    if (! EatIfPossible<TokenType::Identifier>(begin))
        Throw("Not an Identifier of Alias type", *begin);
    return begin;
}

TokenStream::const_iterator Component::Condition::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Condition\n";

    auto expr = make_shared<Expression>();
    begin = expr->Parse(begin, end);

    if ( ! (EatIfPossible<TokenType::Equal>(begin) ||
            EatIfPossible<TokenType::Unequal>(begin) ||
            EatIfPossible<TokenType::GreaterEqual>(begin) ||
            EatIfPossible<TokenType::LessEqual>(begin) ||
            EatIfPossible<TokenType::Greater>(begin) ||
            EatIfPossible<TokenType::Less>(begin)))
            {
                Throw("A comparision operator is expected.", *begin);
            } 

    begin = expr->Parse(begin, end);

    std::cout<<"Condition Parsed\n";
    return begin;
}