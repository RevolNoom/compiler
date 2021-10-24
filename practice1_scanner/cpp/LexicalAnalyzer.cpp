#include "LexicalAnalyzer.hpp"
#include <algorithm>
#include <exception>

using namespace std;

// P U B L I C  F U N C T I O N S

// Don't worry about this un-freed piece of memory
// It's harmless. The kernel will dust it away
LexicalAnalyzer* LexicalAnalyzer::_singleton = new LexicalAnalyzer;

LexicalAnalyzer* LexicalAnalyzer::singleton()
{
	return _singleton;
}

LexicalAnalyzer::LexicalAnalyzer()
{
	_scanChar = decltype(_scanChar)(256, &LexicalAnalyzer::ScanIllegal);

	LexicalAnalyzer::_scanChar['!'] = &LexicalAnalyzer::ScanUnequal;
	LexicalAnalyzer::_scanChar['\'']= &LexicalAnalyzer::ScanCharConst;
	LexicalAnalyzer::_scanChar['('] = &LexicalAnalyzer::ScanLeftParenthesis;

	for (auto c: " \t\n\v\f\r")
		LexicalAnalyzer::_scanChar[c] = &LexicalAnalyzer::ScanSpace;

	for (auto c: "+-/*=,;)><:.")
		LexicalAnalyzer::_scanChar[c] = &LexicalAnalyzer::ScanSymbol;

	for (auto c: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
		LexicalAnalyzer::_scanChar[c] = &LexicalAnalyzer::ScanIdentifier;

	for (auto c: "0123456789")
		LexicalAnalyzer::_scanChar[c] = &LexicalAnalyzer::ScanNumber;
	
	// Symbol - Token

	_keyword["+"] = make_shared<Plus>(0, 0);
	_keyword["-"] = make_shared<Minus>(0, 0);
	_keyword["*"] = make_shared<Times>(0, 0);
	_keyword["/"] = make_shared<Slash>(0, 0);
	_keyword["="] = make_shared<Equal>(0, 0);
	_keyword[","] = make_shared<Comma>(0, 0);
	_keyword[":"] = make_shared<Colon>(0, 0);
	_keyword["."] = make_shared<Period>(0, 0);
	_keyword[";"] = make_shared<Semicolon>(0, 0);
	_keyword["("] = make_shared<ParenthesisOpen>(0, 0);
	_keyword[")"] = make_shared<ParenthesisClose>(0, 0);

	_keyword[":="] = make_shared<Assign>(0, 0);
	_keyword["!="] = make_shared<Unequal>(0, 0);
	_keyword["(."] = make_shared<ArrayIndexOpen>(0, 0);
	_keyword[".)"] = make_shared<ArrayIndexClose>(0, 0);

	_keyword["<"] = make_shared<Less>(0, 0);
	_keyword[">"] = make_shared<Greater>(0, 0);
	_keyword["<="] = make_shared<LessEqual>(0, 0);
	_keyword[">="] = make_shared<GreaterEqual>(0, 0);

	// Keywords - Token 
	_keyword["OF"] = make_shared<OF>(0, 0);
	_keyword["IF"] = make_shared<IF>(0, 0);
	_keyword["DO"] = make_shared<DO>(0, 0);
	_keyword["TO"] = make_shared<TO>(0, 0);
	_keyword["VAR"] = make_shared<VAR>(0, 0);
	_keyword["END"] = make_shared<END>(0, 0);
	_keyword["FOR"] = make_shared<FOR>(0, 0);
	_keyword["TYPE"] = make_shared<TYPE>(0, 0);
	_keyword["CHAR"] = make_shared<CHAR>(0, 0);
	_keyword["CALL"] = make_shared<CALL>(0, 0);
	_keyword["THEN"] = make_shared<THEN>(0, 0);
	_keyword["ELSE"] = make_shared<ELSE>(0, 0);
	_keyword["WHILE"] = make_shared<WHILE>(0, 0);
	_keyword["CONST"] = make_shared<CONST>(0, 0);
	_keyword["ARRAY"] = make_shared<ARRAY>(0, 0);
	_keyword["BEGIN"] = make_shared<BEGIN>(0, 0);
	_keyword["INTEGER"] = make_shared<INTEGER>(0, 0);
	_keyword["PROGRAM"] = make_shared<PROGRAM>(0, 0);
	_keyword["PROCEDURE"] = make_shared<PROCEDURE>(0, 0);

}

TokenStream LexicalAnalyzer::Scan(istream& input)
{
	_line = _col = 1;

	TokenStream result;

	while (input.good() && input.peek() != EOF)
	{
		// Call the appropriate member function 
		// to handle the character
		// Then fetch the token to result
		result.emplace_back((this->*_scanChar[input.peek()])(input));
	}

	return result;
}


// H E L P E R  F U N C T I O N S

shared_ptr<Token> LexicalAnalyzer::ScanIdentifier(istream& input)
{
	string token;
	for (int iii=0; iii<16 && isalnum(input.peek()); ++iii)
		token += GetChar(input);

	if (token.size() > 15)
		Throw("Identifier longer than 15 characters");
	

	string keyword = token;
	// Make the string upper-case
	// Case-insensitive
	for (auto &k: keyword)
		k = std::toupper(k);
	
	// Is this Keyword?
	auto itK = _keyword.find(keyword);

	if (itK == _keyword.end())	
		return make_shared<Identifier>(_line, _col, token); // this is an Identifier 
	else
		// this is a Keyword
		return itK->second->Clone(_line, _col);
}


shared_ptr<Token> LexicalAnalyzer::ScanNumber(std::istream& input)
{
	string token;

	while (isdigit(input.peek()))
		token += GetChar(input);

	return make_shared<Number>(_line, _col, stoi(token));
}

shared_ptr<Token> LexicalAnalyzer::ScanCharConst(istream& input)
{
	GetChar(input);

	if (!isprint(input.peek()))
		Throw(string("Unprintable Character. Charcode: ")+=to_string(input.peek()));

	auto charcode = GetChar(input);

	if (input.peek()!='\'')
		Throw("Missing closing tick for char constant.");

	GetChar(input);

	return make_shared<Char>(_line, _col, charcode);
}

shared_ptr<Token> LexicalAnalyzer::ScanSymbol(istream& input)
{
	string token;

	// Get 2 symbols at a time
	token += GetChar(input);
	token += input.peek();	// Dunno for sure if it's 2-char symbol
							// So stay safe! 

	auto kw = _keyword.find(token);

	if (kw != _keyword.end())
	{
		// We are lucky. These two guys actually make up one symbol

		// Officially get the remaining character 
		GetChar(input);

		return kw->second->Clone(_line, _col);
	}

	// Code flow reaches here, this is one-character symbol
	token.pop_back();
	return _keyword[token]->Clone(_line, _col);
}

shared_ptr<Token> LexicalAnalyzer::ScanSpace(istream& input)
{
	// Discards all white space
	while (isspace(input.peek()))
		GetChar(input);
	return make_shared<Space>(_line, _col);
}

shared_ptr<Token> LexicalAnalyzer::ScanUnequal(istream& input)
{
	string token;
	token += GetChar(input);
	if (input.peek()=='=')
		token += GetChar(input);
	else 
		Throw("\'!\' not followed by \'=\'.");

	return make_shared<Unequal>(_line, _col);
}

shared_ptr<Token> LexicalAnalyzer::ScanLeftParenthesis(istream& input)
{
	string token;
	token += GetChar(input);

	if (input.peek() == '.' )
		token += GetChar(input);

	// Extract ( and (. and end
	if (input.peek() != '*')
		return _keyword[token]->Clone(_line, _col);


	// If it reaches here, this is a comment
	// Ignore everything in comment
	GetChar(input);	// Discard '*' in the stream

	int c;
	while (1)
	{
		c=GetChar(input);
		if (c == '*' && input.peek()==')') // Out of comment condition
		{
			GetChar(input);	// Discard ')'
			break;	// and then we're out 
		}
		else if (c == EOF)
			Throw("File reaches EOF before comment finishes");
	}
	return make_shared<Space>(_line, _col);
}


shared_ptr<Token> LexicalAnalyzer::ScanIllegal(istream& input)
{
	string msg("Illegal char: ");
	msg += input.peek();
	Throw(msg);
	return make_shared<Space>(0, 0);	// Never called
}

void LexicalAnalyzer::Throw(string msg)
{
	msg = "On line " + to_string(_line) + ", col " + to_string(_col) + ":\n" + msg;
	throw invalid_argument(msg);
}

int LexicalAnalyzer::GetChar(istream& input)
{
	if (input.peek() == '\n')
	{
		++_line;
		_col=1;
	}
	else 
		++_col;
	return input.get();
}