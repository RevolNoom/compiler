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


shared_ptr<ProgramComponent> Component::ScopeDependent::Lookup(int DeclarationType, shared_ptr<TokenType::Identifier> id)
{
    // ScopeDependent does not hold a symbol table, so it has to pass the work to its parent
    return _outerScope->Lookup(DeclarationType, id);
}

void Component::Scope::PrintSymbolTable(int tab)
{
    string prefixTab = string(tab+1, '\t');
    string prefixTabPlus = string(tab+2, '\t');
    std::cout<<string(tab, '\t') <<_id->ToString()<<"'s Symbol Table: \n";
    std::cout<<prefixTab<<"Variables:\n";
    for (auto Variable: _symbolTable[DECL_VAR])
    {
        auto var = dynamic_pointer_cast<Component::Type>(Variable.second);
        std::cout<<prefixTabPlus<<Variable.first->ToString()<<": "<<var->GetTypeName()<<"\n";
    }

    std::cout<<prefixTab<<"Constants:\n";
    for (auto Const: _symbolTable[DECL_CONST])
    {
        auto cnst = dynamic_pointer_cast<Component::BasicType>(Const.second);
        std::cout<<prefixTabPlus<<Const.first->ToString()<<": "<<cnst->ToString()<<"\n";
    }

    std::cout<<prefixTab<<"Types:\n";
    for (auto T: _symbolTable[DECL_TYPE])
    {
        auto t = dynamic_pointer_cast<Component::Type>(T.second);
        std::cout<<prefixTabPlus<<T.first->ToString()<<": "<<t->GetTypeName()<<"\n";
    }

    std::cout<<prefixTab<<"Procedures:\n";
    for (auto Proc: _symbolTable[DECL_PROC])
    {
        auto proc = dynamic_pointer_cast<Component::Procedure>(Proc.second);
        proc->PrintSymbolTable(tab+2);
    }

    std::cout<<prefixTab<<"Functions:\n";
    for (auto Func: _symbolTable[DECL_FUNC])
    {
        auto func = dynamic_pointer_cast<Component::Function>(Func.second);
        func->PrintSymbolTable(tab+2);
    }
}

shared_ptr<ProgramComponent> Component::Scope::Lookup(int DeclarationType, shared_ptr<TokenType::Identifier> id)
{
    for (auto p: _symbolTable[DeclarationType])
        if (p.first->ToString() == id->ToString())
            return p.second;
    return nullptr;
}


TokenStream::const_iterator Component::Scope::RegisterDeclarations(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Declarations\n";

    begin = RegisterConst(begin, end);
    begin = RegisterType(begin, end);
    begin = RegisterVar(begin, end);
    begin = RegisterProcedure(begin, end);
    begin = RegisterFunction(begin, end);

    std::cout<<"Declarations Parsed\n";

    begin = RegisterBeginEnd(begin, end);

    return begin;
}

void Component::Scope::RegisterSymbol(int DeclarationType, shared_ptr<TokenType::Identifier> symbol, shared_ptr<ProgramComponent> content)
{
    // Test for naming conflict in the current symbolTable
    for (auto eachDeclarationType: _symbolTable)
        for (auto eachSymbol: eachDeclarationType)
            if (eachSymbol.first->ToString() == symbol->ToString())
                Throw("Symbol conflict", symbol);
    // Otherwise, add it to the table
    _symbolTable[DeclarationType].push_back({symbol, content});
}

TokenStream::const_iterator Component::Scope::RegisterConst(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Const Declaration\n";

    // This declaration is optional
    if (EatIfPossible<TokenType::Const>(begin))
    {
        do
        {
            auto new_const_id = Eat<TokenType::Identifier>(begin, "Missing Identifier for constant declaration.");

            Eat<TokenType::Assign>(begin, "Missing assign symbol.");

            // TODO: Is this unary operator worth noting or not?
            if (EatIfPossible<TokenType::Plus>(begin) || EatIfPossible<TokenType::Minus>(begin)) 
            {
                shared_ptr<Component::Type> ty;

                // Assign the value of this constant with value of another declared constant
                if (ThisTokenIs<TokenType::Identifier>(begin))
                {
                    shared_ptr<TokenType::Identifier> other_constant_id = Eat<TokenType::Identifier>(begin, "");
                    shared_ptr<ProgramComponent> value;
                    if ( (value = Lookup(DECL_VAR, other_constant_id)) ||
                        (value = Lookup(DECL_CONST, other_constant_id))) 
                        RegisterSymbol(DECL_CONST, new_const_id, value);
                    else
                        Throw(string("Unregistered Constant: ") + other_constant_id->ToString(), other_constant_id);
                }
                // Assign this constant var with a raw value
                else if (ty = BasicType::Guess(begin, end))
                {
                    begin = ty->Parse(begin, end);
                    RegisterSymbol(DECL_CONST, new_const_id, ty);
                }
            }
            Eat<TokenType::Semicolon>(begin, "Missing semicolon."); 

        }
        while (ThisTokenIs<TokenType::Identifier>(begin));
    }

    std::cout<<"Const Declaration Parsed\n";
    return begin;
}



TokenStream::const_iterator Component::Scope::RegisterType(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Types Declaration.\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Type>(begin))
    {
        do
        {
            auto id = Eat<TokenType::Identifier>(begin, "Missing Identifier for Type declaration.");

            Eat<TokenType::Assign>(begin, "Missing assign symbol.");

            auto type = Type::Guess(begin, end);
            begin = type->Parse(begin, end);

            Eat<TokenType::Semicolon>(begin, "Missing semicolon.");

            // Add this new type to symbol table
            RegisterSymbol(DECL_TYPE, id, type);

        } while (ThisTokenIs<TokenType::Identifier>(begin));
    }
    std::cout<<"Type Declaration Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::Scope::RegisterVar(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Var Declaration.\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Var>(begin))
    {
        do
        {
            auto id = Eat<TokenType::Identifier>(begin, "Missing Identifier for VAR declaration.");

            Eat<TokenType::Colon>(begin, "Missing colon \':\'.");

            auto type = Type::Guess(begin, end); 
            begin = type->ParseTypename(begin, end);

            Eat<TokenType::Semicolon>(begin, "Missing semicolon."); 

            // Add this new variable to symbol table
            RegisterSymbol(DECL_VAR, id, type);
        }
        while (ThisTokenIs<TokenType::Identifier>(begin));
    }

	std::cout<<"Var Declaration Parsed.\n";
    return begin;
}

TokenStream::const_iterator Component::Scope::RegisterProcedure(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Procedure Declaration\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (ThisTokenIs<TokenType::Procedure>(begin))
    {
        auto proc = make_shared<Component::Procedure>();
        begin = proc->Parse(begin, end);

	    std::cout<<"Is this symbol registered?\n";
        RegisterSymbol(DECL_PROC, proc->GetIdentifier(), proc);
	    std::cout<<"Yes\n";
    }

	std::cout<<"Procedure Declaration Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Scope::RegisterFunction(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Function Declaration\n";

    // This declaration is optional
    if (ThisTokenIs<TokenType::Function>(begin))
    {
        auto func = make_shared<Component::Function>();
        begin = func->Parse(begin, end);

        RegisterSymbol(DECL_FUNC, func->GetIdentifier(), func);
    }

	std::cout<<"Function Declaration Parsed\n";
    return begin;
}

string Component::Function::GetTypeName()
{
    return "Function";
}

TokenStream::const_iterator Component::Function::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Function Declaration\n";

    // Don't throw if the next token isn't your keyword
    // Because this declaration is optional
    // Silently skip parsing. No harm done.
    if (EatIfPossible<TokenType::Function>(begin))
    {
        _id = Eat<TokenType::Identifier>(begin, "Missing Function Identifier.");

        begin = ParseParamList(begin, end);

        Eat<TokenType::Colon>(begin, "Missing Colon ':' before return type.");

        _returnType = BasicType::Guess(begin, end);
        begin = _returnType->Parse(begin, end);

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon.");

        begin = RegisterDeclarations(begin, end);

        Eat<TokenType::Semicolon>(begin, "Missing Semicolon at the function declaration end.");
    }

	std::cout<<"Function Declaration Parsed\n";
    return begin;
}

shared_ptr<Component::Type> Component::Function::Clone()
{
    Throw("Compiler Bug (?): Why would you want to clone a function?");
    return nullptr;
}

TokenStream::const_iterator Component::Function::ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    Throw("Compiler Bug: There's no Function constant. Or is there?", *begin);
    return begin;
}

TokenStream::const_iterator Component::Function::ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    Throw("Compiler Bug: Function aren't supposed to parse type name. Where did you call it?", *begin);
    return begin;
}


bool Component::Function::IsType(shared_ptr<Component::Type> otherType)
{
    return _returnType->IsType(otherType);
}


TokenStream::const_iterator Component::Scope::RegisterBeginEnd(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parse Begin-End\n";

    _body = make_shared<BeginEnd>();
    begin = _body->Parse(begin, end);

	std::cout<<"Begin-End Parsed\n";
    return begin;
}


TokenStream::const_iterator Component::Program::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
	std::cout<<"Parsing Program\n";

    Eat<TokenType::Program>(begin, "Program does not start with keyword Program");

    _id = Eat<TokenType::Identifier>(begin, "Identifier expected for program name. \'" + (*begin)->GetTokenType() + "\' does not look like a valid Identifier.");
        
    Eat<TokenType::Semicolon>(begin, "Missing Semicolon ';' after program name.");

    begin = RegisterDeclarations(begin, end);
    
    Eat<TokenType::Period>(begin, "Your program misses a period '.' at the end.");

    if (begin != end)
        Throw("Trailing characters after Program.", *begin);
	
    std::cout<<"Program Parsed\n";

    return begin;
}

TokenStream::const_iterator Component::Procedure::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    Eat<TokenType::Procedure>(begin, "Missing keyword Procedure");

    _id = Eat<TokenType::Identifier>(begin, "Missing procedure Identifier.");

    begin = ParseParamList(begin, end);

    Eat<TokenType::Semicolon>(begin, "Missing Semicolon.");

    begin = RegisterDeclarations(begin, end);

    Eat<TokenType::Semicolon>(begin, "Missing Semicolon at the procedure declaration end.");

    return begin;
}


shared_ptr<Component::Type> Component::Type::Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Guessing Type\n";
    if (begin == end)
        Throw("End of file, nothing to guess");

    // Try pinning which type is this
    if (ThisTokenIs<TokenType::Array>(begin))
    {
        std::cout<<"Type Array\n";
        return make_shared<Component::Array>();
    }
    
    if (ThisTokenIs<TokenType::Char>(begin))
    {
        std::cout<<"Type Char\n";
        return BasicType::MakeChar(0);
    }
    
    if (ThisTokenIs<TokenType::Integer>(begin))
    {
        std::cout<<"Type Integer\n";
        return BasicType::MakeInteger(0);
    }

    std::cout<<"Type ???\n";
    return nullptr;
}

bool Component::BasicType::IsType(shared_ptr<Component::Type> otherType)
{
    auto basicType = dynamic_pointer_cast<BasicType>(otherType);
    return basicType && 
            basicType->_typeTag == _typeTag;

}

shared_ptr<Component::Type> Component::BasicType::Clone()
{
    auto result = make_shared<BasicType>();
    result->_typeTag = _typeTag;
    result->_value = _value;
    return result;
}


TokenStream::const_iterator Component::Array::ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing Array\n";

    Eat<TokenType::Array>(begin, "Unknown type.");
    Eat<TokenType::ArrayIndexOpen>(begin, "Missing array index symbol '(.'");

    // Get Array size
    _size = Eat<TokenType::Number>(begin, "Array size not specified")->GetValue();

    Eat<TokenType::ArrayIndexClose>(begin, "Missing array index symbol '.)'");
    Eat<TokenType::Of>(begin, "Missing keyword Of");

    // Get the data type Array stores
    _elementType = Type::Guess(begin, end);
    begin = _elementType->Parse(begin, end);

    std::cout<<"Array Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::Array::ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    Throw("Compiler Bug: Can't parse constant values for an Array", *begin);
    return begin;
}


bool Component::Array::IsType(shared_ptr<Component::Type> otherType)
{
    auto ComparedArray = dynamic_pointer_cast<Component::Array>(otherType);
    return ComparedArray && 
            _elementType->IsType(ComparedArray->GetElementType()); 
}


shared_ptr<Component::Type> Component::Array::Clone()
{
    auto result = make_shared<Component::Array>();
    result->_size = _size;
    result->_elementType = _elementType;
    return result;
}

TokenStream::const_iterator Component::Procedure::ParseParamList(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Parsing ParamList\n";

    // Param Declaration is optional
    if (EatIfPossible<TokenType::ParenthesisOpen>(begin))
    {
        do
        {
            EatIfPossible<TokenType::Var>(begin);

            auto id = Eat<TokenType::Identifier>(begin, "Missing Identifier.");

            Eat<TokenType::Colon>(begin, "Missing colon ':'.");
            
            auto bstype = BasicType::Guess(begin, end);
            begin = bstype->ParseTypename(begin, end);

            RegisterSymbol(DECL_VAR, id, bstype);
        }
        while (EatIfPossible<TokenType::Semicolon>(begin));
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

        auto stm = Statement::Guess(begin, end);
        begin = stm->Parse(begin, end);

        if (EatIfPossible<TokenType::Else>(begin))
        {
            auto stm = Statement::Guess(begin, end);
            begin = stm->Parse(begin, end);
        }
    }

    std::cout<<"If Parsed\n";
    return begin;
}

TokenStream::const_iterator Component::BeginEnd::Parse(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    Eat<TokenType::Begin>(begin, "Missing Begin-End block");

    do
    {
        std::cout<<"Error over here\n";
        begin = Statement::Guess(begin, end)->Parse(begin, end);
        std::cout<<"Error \n";
    }
    while (EatIfPossible<TokenType::Semicolon>(begin));

    Eat<TokenType::End>(begin, "Missing End keyword");
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

        auto stm = Statement::Guess(begin, end);
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

        auto stm = Statement::Guess(begin, end);
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

shared_ptr<Statement> Component::Statement::Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    std::cout<<"Guessing Statement type\n";
    if (begin == end)
        return nullptr;

    if (ThisTokenIs<TokenType::If>(begin))
        return make_shared<Component::If>();
        
    if (ThisTokenIs<TokenType::Call>(begin))
        return make_shared<Component::Call>();

    if (ThisTokenIs<TokenType::For>(begin))
        return make_shared<Component::For>();

    if (ThisTokenIs<TokenType::While>(begin))
        return make_shared<Component::While>();

    if (ThisTokenIs<TokenType::Identifier>(begin))
        return make_shared<Component::Assignment>();

    if (ThisTokenIs<TokenType::Begin>(begin))
        return make_shared<Component::BeginEnd>();

    if (ThisTokenIs<TokenType::Semicolon>(begin) ||
        ThisTokenIs<TokenType::End>(begin))
        return make_shared<Component::EmptyStatement>();

    return nullptr;
}


// T Y P E S
shared_ptr<Component::BasicType> Component::BasicType::Guess(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    if (ThisTokenIs<TokenType::Char>(begin) || ThisTokenIs<TokenType::Letter>(begin))
        return BasicType::MakeChar(0);
    
    if (ThisTokenIs<TokenType::Integer>(begin) || ThisTokenIs<TokenType::Number>(begin))
        return BasicType::MakeInteger(0);

    return nullptr;
}

TokenStream::const_iterator Component::BasicType::ParseTypename(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    if (_typeTag == INTEGER)
        Eat<TokenType::Integer>(begin, "Missing Integer keyword");
    else if (_typeTag == CHAR)
        Eat<TokenType::Char>(begin, "Missing Char keyword");
    else
        Throw("Compiler Bug: BasicType doesn't have a typetag yet");
    return begin;
}

TokenStream::const_iterator Component::BasicType::ParseValue(TokenStream::const_iterator begin, TokenStream::const_iterator end)
{
    if (_typeTag == INTEGER)
        _value._i = Eat<TokenType::Number>(begin, string("Can't assign a ") + (*begin)->GetTokenType() + string(" to an Integer constant"))->GetValue();
    else if (_typeTag == CHAR)
        _value._c = Eat<TokenType::Letter>(begin, string("Can't assign a ") + (*begin)->GetTokenType() + string(" to a Char constant"))->GetValue();
    else
        Throw("Compiler Bug: BasicType doesn't have a typetag yet");
    return begin;
}

shared_ptr<Component::BasicType> Component::BasicType::MakeChar(char value)
{
    auto bs = make_shared<BasicType>();
    bs->_typeTag = BasicType::CHAR;
    bs->_value._c = value;
    return bs;
}

shared_ptr<Component::BasicType> Component::BasicType::MakeInteger(int value)
{
    auto bs = make_shared<BasicType>();
    bs->_typeTag = BasicType::INTEGER;
    bs->_value._i = value;
    return bs;
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