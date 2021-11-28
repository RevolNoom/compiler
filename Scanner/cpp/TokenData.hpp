#ifndef TOKEN_DATA_HPP
#define TOKEN_DATA_HPP

#include "Token.hpp"
#include <string>

using namespace std;

class Identifier: public Token
{
public:

    string GetTokenType() const override  {return "IDENTIFIER";}
    string ToString() const override   {return _id;}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Identifier>(line, col, _id);
    }

    Identifier(int line, int col, string id): Token(line, col), _id(id)
    {}

private:
    string _id;
};




class Number: public Token
{
public:

    string GetTokenType() const override  {return "NUMBER";}
    string ToString() const override   {return to_string(_value);}
    // TODO: GetValue()?
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Number>(line, col, _value);
    }

    Number(int line, int col, int value): Token(line, col), _value(value)
    {}

private:
    int _value;
};




class Char: public Number 
{
public:

    string GetTokenType() const override  {return "CHAR";}
    string ToString() const override   
    {
        auto result = string("\'x\'");
        result[1]=_charcode;
        return result;
    }

    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Char>(line, col, _charcode);
    }

    Char(int line, int col, int charcode): Number(line, col, charcode)
    {}

private:
    int _charcode;
};

#endif