#include <stdexcept>
#include "Token.hpp"
#include "Parser.hpp"

using namespace ParserType;
using namespace TokenType;

TopDown* TopDown::_singleton = nullptr;

Parser* TopDown::Singleton()
{
    if (_singleton == nullptr)
        _singleton = new TopDown();

    return _singleton;
}

TopDown::TopDown()
{}

void TopDown::Parse(TokenStream &ts) const
{
	std::cout<<"\n...Parsing...\n";

	ParseProgram(ts);
}


void TopDown::ParseProgram(TokenStream &ts) const
{
	std::cout<<"\nParseProgram\n";

    // Syntax:
    // PROGRAM IDENTIFIER: BLOCK .

    Eat<Program>(ts, "Program does not start with keyword Program");

    Eat<Identifier>(ts, "Identifier expected for program name. \'" + ts.front()->GetTokenType() + "\' does not look like a valid Identifier.");
        
    Eat<Semicolon>(ts, "Missing Semicolon ';' after program name.");

    ParseBlock(ts);
    
    Eat<Period>(ts, "Your program misses a period '.' in the end.");
	std::cout<<"\nParseProgram Completed\n";
}

void TopDown::ParseBlock(TokenStream &ts) const
{
	std::cout<<"\nParseBlock\n";

    // Declarations. STRICTLY in this order: CONST TYPE VAR PROCEDURE FUNCTION

        if (TheNextTokenIs<Const>(ts))
            ParseDeclConst(ts);

        if (TheNextTokenIs<Type>(ts))
            ParseDeclType(ts);

        if (TheNextTokenIs<Var>(ts))
            ParseDeclVar(ts);

        if (TheNextTokenIs<Procedure>(ts))
            ParseDeclProcedure(ts);

        if (TheNextTokenIs<Function>(ts))
            ParseDeclFunction(ts);

    // BEGIN-END BLOCK

        if (TheNextTokenIs<Begin>(ts))
            ParseBeginEnd(ts);

	std::cout<<"\nParseBlock Completed\n";

}

void TopDown::ParseDeclConst(TokenStream &ts) const
{
	std::cout<<"\nParseDeclConst\n";

    Eat<Const>(ts, "Const declaration does not start with \"Const\" but " + ts.front()->GetTokenType());

    // Gobble up all const assignments 
    while (1)
    {
        Eat<Identifier>(ts, "Missing Identifier for constant declaration.");

        Eat<Assign>(ts, "Missing assign symbol.");

        /*
            A Constant:
            + A Letter
            or
            + Plus/Minus symbol followed by ConstantIdentifier/UnsignedInt
        */
            if (TheNextTokenIs<Letter>(ts))
                Eat<Token>(ts, "");
            else
            {
                if (TheNextTokenIs<Plus>(ts) ||
                    TheNextTokenIs<Minus>(ts))
                    Eat<Token>(ts, "");

                Eat<Number>(ts, string("Not a number constant. It's a ") + ts.front()->GetTokenType());
            }
        
        // Semicolon ';'
        Eat<Semicolon>(ts, "Missing semicolon."); 

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }
	std::cout<<"\nParseDeclConst Completed\n";
}

void TopDown::ParseDeclType(TokenStream &ts) const
{
	std::cout<<"\nParseDeclType\n";

    Eat<Type>(ts, "Type declaration does not start with \"Type\" but " + ts.front()->GetTokenType());

    while (1)
    {
        Eat<Identifier>(ts, "Missing Identifier for Type declaration.");

        Eat<Assign>(ts, "Missing assign symbol.");

        ParseType(ts);

        Eat<Semicolon>(ts, "Missing semicolon.");

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }
	std::cout<<"\nParseDeclType Completed\n";
}

// Almost the same with DeclConst 
void TopDown::ParseDeclVar(TokenStream &ts) const
{
	std::cout<<"\nParseDeclVar\n";

    Eat<Var>(ts, "VAR declaration does not start with \"VAR\" but " + ts.front()->GetTokenType());

    while (1)
    {
        Eat<Identifier>(ts, "Missing Identifier for VAR declaration.");

        Eat<Colon>(ts, "Missing colon \':\'.");

        ParseType(ts);

        Eat<Semicolon>(ts, "Missing semicolon."); 

        // Loop if the next Token is an Identifier.
        if (!TheNextTokenIs<Identifier>(ts))
            break;
    }
	std::cout<<"\nParseDeclVar Completed\n";
}

void TopDown::ParseDeclProcedure(TokenStream &ts) const
{
	std::cout<<"\nParseDeclProcedure\n";

    Eat<Procedure>(ts, "Procedure declaration does not start with \"PROCEDURE\" but " + ts.front()->GetTokenType());

    Eat<Identifier>(ts, "Missing procedure identifier.");

    // A ParamList is optional 
    if (TheNextTokenIs<ParenthesisOpen>(ts))
        ParseParamList(ts);

    Eat<Semicolon>(ts, "Missing Semicolon.");

    ParseBlock(ts);

    Eat<Semicolon>(ts, "Missing Semicolon at the procedure declaration end.");
	std::cout<<"\nParseDeclProcedure Completed\n";
}


void TopDown::ParseDeclFunction(TokenStream &ts) const
{
	std::cout<<"\nParseDeclFunction\n";

    Eat<Function>(ts, "Function declaration does not start with \"FUNCTION\" but " + ts.front()->GetTokenType());

    Eat<Identifier>(ts, "Missing function identifier.");

    // A ParamList is optional 
    if (TheNextTokenIs<ParenthesisOpen>(ts))
        ParseParamList(ts);

    Eat<Colon>(ts, "Missing Colon.");

    ParseTypeBasic(ts);

    Eat<Semicolon>(ts, "Missing Semicolon after return type.");

    ParseBlock(ts);

    Eat<Semicolon>(ts, "Missing Semicolon at the function declaration end.");
	std::cout<<"\nParseDeclFunction Completed\n";
}

void TopDown::ParseBeginEnd(TokenStream &ts) const
{
	std::cout<<"\nParseBeginEnd\n";

    Eat<Begin>(ts, "Begin-End block does not start with \"BEGIN\" but " + ts.front()->GetTokenType());

    while (1)
    {
        ParseStatement(ts);

        // Break out of Begin-End block when we reach End
        if (!TheNextTokenIs<Semicolon>(ts))
            break;

        Eat<Semicolon>(ts, "Missing semicolon at the end of statement.");
    }

    Eat<End>(ts, "Missing End Keyword. Or did you forgot a semicolon ';'?");
	std::cout<<"\nParseBeginEnd Completed\n";
}



// LESS EVIL PARSINGS


void TopDown::ParseType(TokenStream &ts) const
{
	std::cout<<"\nParseType\n";

    // TypeIdentifier and BasicTypes
    if (TheNextTokenIs<Identifier>(ts) ||   
        TheNextTokenIs<Char>(ts) ||
        TheNextTokenIs<Integer>(ts))
        {
            Eat<Token>(ts, "");
	        std::cout<<"\nParseType Completed\n";
        }

    // ArrayType
    else if (TheNextTokenIs<Array>(ts))
    {
        Eat<Array>(ts, "");

        Eat<ArrayIndexOpen>(ts, "Missing array opening token.");

        Eat<Number>(ts, "You forgot to specify array size.");

        Eat<ArrayIndexClose>(ts, "Missing array closing token.");

        Eat<Of>(ts, "Did you forget keyword OF?");

        // Recursively parse array type
        ParseType(ts);
	    std::cout<<"\nParseType Completed\n";
    }
    else
        Throw("Type Declaration failed: Unknown type. It's a " + ts.front()->GetTokenType(), ts.front());
}

void TopDown::ParseTypeBasic(TokenStream &ts) const
{
	std::cout<<"\nParseTypeBasic\n";

    if (TheNextTokenIs<Char>(ts) ||
        TheNextTokenIs<Integer>(ts))
        Eat<Token>(ts, "");
    else
        Throw(ts.front()->GetTokenType() + " is not a basic Type (Integer, Char).", ts.front());

    std::cout<<"\nParseTypeBasic Completed\n";
}

void TopDown::ParseParamList(TokenStream &ts) const
{
	std::cout<<"\nParseParamList\n";

    Eat<ParenthesisOpen>(ts, "Parameter list did not start with open parenthesis. It was " + ts.front()->GetTokenType());

    while (2)
    {
        if (TheNextTokenIs<Var>(ts))    // Ignore VAR
            Eat<Token>(ts, "");

        Eat<Identifier>(ts, "Missing Identifier.");

        Eat<Colon>(ts, "Missing Colon.");

        ParseTypeBasic(ts);

        // Finish parsing paramlist if we encounter a close
        if (!TheNextTokenIs<Semicolon>(ts))
            break;

        // Continue parsing if it's a semicolon
        Eat<Semicolon>(ts, "Missing Semicolon to separate arguments.");
    }
    Eat<ParenthesisClose>(ts, "Missing closing Parenthesis");
	std::cout<<"\nParseParamList Completed\n";
}


// TODO:
void TopDown::ParseStatement(TokenStream &ts) const
{
	std::cout<<"\nParseStatement\n";

    if (TheNextTokenIs<Identifier>(ts))
        ParseAssignment(ts);
    else if (TheNextTokenIs<Call>(ts))
        ParseCallProc(ts);
    else if (TheNextTokenIs<If>(ts))
        ParseIfElse(ts);
    else if (TheNextTokenIs<While>(ts))
        ParseWhile(ts);
    else if (TheNextTokenIs<For>(ts))
        ParseFor(ts);
    else if (TheNextTokenIs<Begin>(ts))
        ParseBeginEnd(ts);
    
    // If everything above doesn't work, this is a null statement
	std::cout<<"\nParseStatement Completed\n";
}

void TopDown::ParseAssignment(TokenStream &ts) const
{
	std::cout<<"\nParseAssignment\n";

    // Variable/FunctionIdentifier
    Eat<Identifier>(ts, "You kidding me? Who wrote this code? This ain't no Identifier! It's a "+ts.front()->GetTokenType());

    // If it's a FunctionIdentifier, nothing interesting happens
    // But if it's a Variable, it could be array access
    // Let's test that
    while (TheNextTokenIs<ArrayIndexOpen>(ts))
    {
        Eat<Token>(ts, "");

        ParseExpression(ts);

        Eat<ArrayIndexClose>(ts, "Missing Array closing token");
    }

    Eat<Assign>(ts, "Assignment with "+ts.front()->GetTokenType()+" instead of ':=' symbol. You are funny.");

    ParseExpression(ts);
	std::cout<<"\nParseAssignment Completed\n";
}

void TopDown::ParseCallProc(TokenStream &ts) const
{
	std::cout<<"\nParseCallProc\n";

    Eat<Call>(ts, "You called me, but you didn't CALL me.");

    Eat<Identifier>(ts, "Missing procedure Identifier");

    // TODO: I need to ask again to make sure that "()" is optional
    if (TheNextTokenIs<ParenthesisOpen>(ts))
    {
        Eat<ParenthesisOpen>(ts, "Missing opening parenthesis");

        while (1)
        {
            ParseExpression(ts);

            // No more params?
            if (!TheNextTokenIs<Comma>(ts))
                break;

            // There's still some more parameters
            Eat<Comma>(ts, "Missing comma to separate arguments.");
        }

        // When the paramlist end
        Eat<ParenthesisClose>(ts, "Missing closing parenthesis for parameters list");
    }
	std::cout<<"\nParseCallProc Completed\n";
}

void TopDown::ParseIfElse(TokenStream &ts) const
{
	std::cout<<"\nParseIfElse\n";

    Eat<If>(ts, "If else but missing IF");

    ParseCondition(ts);

    Eat<Then>(ts, "Missing THEN before statements");

    ParseStatement(ts);

    // Optional:
    if (TheNextTokenIs<Else>(ts))
    {
        Eat<Token>(ts, "");
        ParseStatement(ts);
    }
	std::cout<<"\nParseIfElse Completed\n";
}

void TopDown::ParseWhile(TokenStream &ts) const
{
	std::cout<<"\nParseWhile\n";

    Eat<While>(ts, "You forget WHILE.");

    ParseCondition(ts);

    Eat<Then>(ts, "Missing THEN before statements");

    ParseStatement(ts);
	std::cout<<"\nParseWhile completed\n";
}

void TopDown::ParseFor(TokenStream &ts) const
{
	std::cout<<"\nParseFor\n";

    Eat<For>(ts, "This error is FOR you. You forgot FOR.");

    Eat<Identifier>(ts, "An Identifier is expected here.");

    Eat<Assign>(ts, "Missing assignment symbol ':='.");

    ParseExpression(ts);

    Eat<To>(ts, "You forgot TO.");

    ParseExpression(ts);

    Eat<Do>(ts, "I forget DO sometimes too.");

    ParseStatement(ts);
	std::cout<<"\nParseFor Completed\n";
}



void TopDown::ParseCondition(TokenStream &ts) const
{
	std::cout<<"\nParseCondition\n";

    ParseExpression(ts);

    if (TheNextTokenIs<Equal>(ts) ||
        TheNextTokenIs<Unequal>(ts) ||

        TheNextTokenIs<Greater>(ts) ||
        TheNextTokenIs<GreaterEqual>(ts) ||

        TheNextTokenIs<Less>(ts) ||
        TheNextTokenIs<LessEqual>(ts)) 

        Eat<Token>(ts, "");

    else
        Throw("Missing comparision in condition.", ts.front());

    ParseExpression(ts);
	std::cout<<"\nParseCondition Completed\n";
}


void TopDown::ParseExpression(TokenStream &ts) const
{
	std::cout<<"\nParseExpression\n";

    if (TheNextTokenIs<Plus>(ts) ||
        TheNextTokenIs<Minus>(ts))
        Eat<Token>(ts, "");     // Don't care + -
    
    ParseTerm(ts);

    while (TheNextTokenIs<Plus>(ts) ||
            TheNextTokenIs<Minus>(ts))
            {
                Eat<Token>(ts, ""); // Remove + -
                ParseTerm(ts);
            }
	std::cout<<"\nParseExpression completed\n";
}

void TopDown::ParseTerm(TokenStream &ts) const
{
	std::cout<<"\nParseTerm\n";

    ParseFactor(ts);
    while (TheNextTokenIs<Times>(ts) ||
            TheNextTokenIs<Slash>(ts))
            {
                Eat<Token>(ts, "");     // Remove * / 
                ParseFactor(ts);
            }
	std::cout<<"\nParseTerm Completed\n";
}

void TopDown::ParseFactor(TokenStream &ts) const
{
	std::cout<<"\nParseFactor\n";

    if (TheNextTokenIs<Letter>(ts) ||
        TheNextTokenIs<Number>(ts))
        {
            Eat<Token>(ts, "");
        }

    /*
        Identifier
        Identifier(.1.)(.2.)(...)
        Identifier(expression, ...)
    */
    else if (TheNextTokenIs<Identifier>(ts))
    {
        Eat<Token>(ts, "");

        // Array access a(.1.)(.2.)(...)
        while (TheNextTokenIs<ArrayIndexOpen>(ts))
        {
            Eat<Token>(ts, "");

            ParseExpression(ts);

            Eat<ArrayIndexClose>(ts, "Array Index not closed");
        }


        // Optional suffix: (expression, ...) 
        if (TheNextTokenIs<ParenthesisOpen>(ts))
        {
            Eat<Token>(ts, ""); // Pop '('

            while (2)
            {
                ParseExpression(ts);

                // We're out of paramlist 
                if (!TheNextTokenIs<Comma>(ts))
                    break;
                Eat<Token>(ts, ""); // Pop ','
            }
            Eat<ParenthesisClose>(ts, "Missing closing parenthesis.");
        }
    }
    else
    {
        // ( expression )
        Eat<ParenthesisOpen>(ts, "An expression is expected."); // Pop '('

        ParseExpression(ts);

        Eat<ParenthesisClose>(ts, "Expecting closing parenthesis for expression."); // Pop ')'
    }
	std::cout<<"\nParseFactor Completed\n";
}
