/*
    A uniform interface of all KPL token 
    OOP pattern: Visitor, Prototype
*/
#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <list>
#include <memory>
#include <string>

using namespace std;

class Token;
using TokenStream = list<shared_ptr<Token>>;

class Token
{
public:

    int GetCol()  {return _col;}
    int GetLine() {return _line;}

    virtual string ToString() const = 0;
    virtual string GetTokenType() const = 0;

    // I don't want to create a family of factory classes just for creation of Token
    // Thus, every token will now be a prototype
    // Return a new token of current type on another line and column 
    virtual shared_ptr<Token> Clone(int line, int col) const = 0;

    // TODO: Parser's (?) part
    // Each type of token creates a specific block of code
    // Extract the stream then build its block of code
    // For example: "If" statement:
    //      If true then 
    //          dosomething 
    //      else...
    // The current token is "if", so the TokenStream should contain 
    // the rest of the program, from "true then dosomething..." 
    //
    // virtual shared_ptr<ABlockOfCode> Build(TokenStream& stream) = 0;

    // Cast this token to the same type with anotherToken
    //virtual std::shared_ptr<Token> CastToSameWith(const Token* const anotherToken) = 0;

protected:
    Token(int line, int col): _col(col), _line(line)
    {
    }

    //virtual std::shared_ptr<Token> CastToChar(const Token* const anotherToken) = 0;
    //virtual std::shared_ptr<Token> CastToNumber(const Token* const anotherToken) = 0;
    //virtual std::shared_ptr<Token> CastToOperator(const Token* const anotherToken) = 0;

    // Generic CastTo. For user-defined types?
    //virtual std::shared_ptr<Token> CastTo(const Token* const anotherToken) = 0;

private:
    int _line, _col;
};
#endif