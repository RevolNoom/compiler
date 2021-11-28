#ifndef KEYWORD_HPP
#define KEYWORD_HPP

#include "Token.hpp"

// Abstract
class KEYWORD: public Token
{
public:
	string GetTokenType() const override	{return "KEYWORD";}

protected:
    KEYWORD(int line, int col): Token(line, col)
    {}
};



// CONCRETE KEYWORDS

class PROGRAM: public KEYWORD
{
public:
	string ToString() const override	{return "PROGRAM";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<PROGRAM>(line, col);
    }


	PROGRAM(int line, int col): KEYWORD(line, col)
	{}
};
 
class CONST: public KEYWORD
{
public:
	string ToString() const override	{return "CONST";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<CONST>(line, col);
    }


	CONST(int line, int col): KEYWORD(line, col)
	{}
};

class TYPE: public KEYWORD
{
public:
	string ToString() const override	{return "TYPE";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<TYPE>(line, col);
    }


	TYPE(int line, int col): KEYWORD(line, col)
	{}
};

class VAR: public KEYWORD
{
public:
	string ToString() const override	{return "VAR";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<VAR>(line, col);
    }


	VAR(int line, int col): KEYWORD(line, col)
	{}
};

class INTEGER: public KEYWORD
{
public:
	string ToString() const override	{return "INTEGER";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<INTEGER>(line, col);
    }


	INTEGER(int line, int col): KEYWORD(line, col)
	{}
};

class CHAR: public KEYWORD
{
public:
	string ToString() const override	{return "CHAR";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<INTEGER>(line, col);
    }


	CHAR(int line, int col): KEYWORD(line, col)
	{}
};

class ARRAY: public KEYWORD
{
public:
	string ToString() const override	{return "ARRAY";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<ARRAY>(line, col);
    }


	ARRAY(int line, int col): KEYWORD(line, col)
	{}
};

class OF: public KEYWORD
{
public:
	string ToString() const override	{return "OF";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<OF>(line, col);
    }


	OF(int line, int col): KEYWORD(line, col)
	{}
};

class FUNCTION: public KEYWORD
{
public:
	string ToString() const override	{return "FUNCTION";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<FUNCTION>(line, col);
    }


	FUNCTION(int line, int col): KEYWORD(line, col)
	{}
};

class PROCEDURE: public KEYWORD
{
public:
	string ToString() const override	{return "PROCEDURE";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<PROCEDURE>(line, col);
    }


	PROCEDURE(int line, int col): KEYWORD(line, col)
	{}
};

class BEGIN: public KEYWORD
{
public:
	string ToString() const override	{return "BEGIN";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<BEGIN>(line, col);
    }


	BEGIN(int line, int col): KEYWORD(line, col)
	{}
};

class END: public KEYWORD
{
public:
	string ToString() const override	{return "END";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<END>(line, col);
    }


	END(int line, int col): KEYWORD(line, col)
	{}
};

class CALL: public KEYWORD
{
public:
	string ToString() const override	{return "CALL";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<CALL>(line, col);
    }


	CALL(int line, int col): KEYWORD(line, col)
	{}
};

class IF: public KEYWORD
{
public:
	string ToString() const override	{return "IF";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<IF>(line, col);
    }


	IF(int line, int col): KEYWORD(line, col)
	{}
};

class THEN: public KEYWORD
{
public:
	string ToString() const override	{return "THEN";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<THEN>(line, col);
    }


	THEN(int line, int col): KEYWORD(line, col)
	{}
};

class ELSE: public KEYWORD
{
public:
	string ToString() const override	{return "ELSE";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<ELSE>(line, col);
    }


	ELSE(int line, int col): KEYWORD(line, col)
	{}
};

class WHILE: public KEYWORD
{
public:
	string ToString() const override	{return "WHILE";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<WHILE>(line, col);
    }


	WHILE(int line, int col): KEYWORD(line, col)
	{}
};

class DO: public KEYWORD
{
public:
	string ToString() const override	{return "DO";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<DO>(line, col);
    }


	DO(int line, int col): KEYWORD(line, col)
	{}
};

class FOR: public KEYWORD
{
public:
	string ToString() const override	{return "FOR";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<FOR>(line, col);
    }


	FOR(int line, int col): KEYWORD(line, col)
	{}
};

class TO: public KEYWORD
{
public:
	string ToString() const override	{return "TO";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<TO>(line, col);
    }


	TO(int line, int col): KEYWORD(line, col)
	{}
};


#endif