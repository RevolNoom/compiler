#ifndef TOKEN_SYMBOL_HPP
#define TOKEN_SYMBOL_HPP

#include "Token.hpp"

// Base Symbol class
// Abstract (almost?)
class Symbol: public Token
{
protected:
	Symbol(int line, int col): Token(line, col)
	{}
};

// WARNING: Maybe I should just throw this class away?
// Any type of space: space, tab, enter, carriage,...
class Space: public Symbol
{
public:
	string GetTokenType() const override	{return "Space";}
	string ToString() const override	{return " ";}

    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Space>(line, col);
    }

	Space(int line, int col): Symbol(line, col)
	{}
};


// CONCRETE SYMBOL CLASSES

class Semicolon: public Symbol
{
public:
	string GetTokenType() const override	{return "Semicolon";}
	string ToString() const override	{return ";";}

    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Semicolon>(line, col);
    }

	Semicolon(int line, int col): Symbol(line, col)
	{}
};

class Colon: public Symbol
{
public:
	string GetTokenType() const override	{return "Colon";}
	string ToString() const override	{return ":";}
    virtual shared_ptr<Token> Clone(int line, int col) const override   
    {
        return make_shared<Colon>(line, col);
    }

	Colon(int line, int col): Symbol(line, col)
	{}
};

class Period: public Symbol
{
public:
	string GetTokenType() const override	{return "Period";}
	string ToString() const override	{return ".";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Period>(line, col);
	}

	Period(int line, int col): Symbol(line, col)
	{}
};

class Comma: public Symbol
{
public:
	string GetTokenType() const override	{return "Comma";}
	string ToString() const override	{return ",";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Comma>(line, col);
	}

	Comma(int line, int col): Symbol(line, col)
	{}
};

class Assign: public Symbol
{
public:
	string GetTokenType() const override	{return "Assign";}
	string ToString() const override	{return ":=";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Assign>(line, col);
	}

	Assign(int line, int col): Symbol(line, col)
	{}
};

class Equal: public Symbol
{
public:
	string GetTokenType() const override	{return "Equal";}
	string ToString() const override	{return "=";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Equal>(line, col);
	}

	Equal(int line, int col): Symbol(line, col)
	{}
};

class Unequal: public Symbol
{
public:
	string GetTokenType() const override	{return "Unequal";}
	string ToString() const override	{return "!=";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Unequal>(line, col);
	}

	Unequal(int line, int col): Symbol(line, col)
	{}
};

class Less: public Symbol
{
public:
	string GetTokenType() const override	{return "Less";}
	string ToString() const override	{return "<";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Less>(line, col);
	}

	Less(int line, int col): Symbol(line, col)
	{}
};

class LessEqual: public Symbol
{
public:
	string GetTokenType() const override	{return "LessEqual";}
	string ToString() const override	{return "<=";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<LessEqual>(line, col);
	}

	LessEqual(int line, int col): Symbol(line, col)
	{}
};

class Greater: public Symbol
{
public:
	string GetTokenType() const override	{return "Greater";}
	string ToString() const override	{return ">";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Greater>(line, col);
	}

	Greater(int line, int col): Symbol(line, col)
	{}
};

class GreaterEqual: public Symbol
{
public:
	string GetTokenType() const override	{return "GreaterEqual";}
	string ToString() const override	{return ">=";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<GreaterEqual>(line, col);
	}

	GreaterEqual(int line, int col): Symbol(line, col)
	{}
};
class Plus: public Symbol
{
public:
	string GetTokenType() const override	{return "Plus";}
	string ToString() const override	{return "+";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Plus>(line, col);
	}

	Plus(int line, int col): Symbol(line, col)
	{}
};

class Minus: public Symbol
{
public:
	string GetTokenType() const override	{return "Minus";}
	string ToString() const override	{return "-";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Minus>(line, col);
	}

	Minus(int line, int col): Symbol(line, col)
	{}
};

class Times: public Symbol
{
public:
	string GetTokenType() const override	{return "Times";}
	string ToString() const override	{return "*";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Times>(line, col);
	}

	Times(int line, int col): Symbol(line, col)
	{}
};

class Slash: public Symbol
{
public:
	string GetTokenType() const override	{return "Slash";}
	string ToString() const override	{return "/";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<Slash>(line, col);
	}

	Slash(int line, int col): Symbol(line, col)
	{}
};
class ParenthesisOpen: public Symbol
{
public:
	string GetTokenType() const override	{return "ParenthesisOpen";}
	string ToString() const override	{return "(";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<ParenthesisOpen>(line, col);
	}

	ParenthesisOpen(int line, int col): Symbol(line, col)
	{}
};

class ParenthesisClose: public Symbol
{
public:
	string GetTokenType() const override	{return "ParenthesisClose";}
	string ToString() const override	{return ")";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<ParenthesisClose>(line, col);
	}

	ParenthesisClose(int line, int col): Symbol(line, col)
	{}
};

class ArrayIndexOpen: public Symbol
{
public:
	string GetTokenType() const override	{return "ArrayIndexOpen";}
	string ToString() const override	{return "(.";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<ArrayIndexOpen>(line, col);
	}

	ArrayIndexOpen(int line, int col): Symbol(line, col)
	{}
};

class ArrayIndexClose: public Symbol
{
public:
	string GetTokenType() const override	{return "ArrayIndexClose";}
	string ToString() const override	{return ".)";}

	virtual shared_ptr<Token> Clone(int line, int col) const override
	{
		return make_shared<ArrayIndexClose>(line, col);
	}

	ArrayIndexClose(int line, int col): Symbol(line, col)
	{}
};



#endif 