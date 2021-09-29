#include "LexicalAnalyzer.hpp"
#include <algorithm>
#include <exception>

using namespace std;

// P U B L I C  F U N C T I O N S

LexicalAnalyzer::LexicalAnalyzer()
{
	InitScanChar();
}

void LexicalAnalyzer::Scan(istream& input)
{
	// In case of fire
	// Please Use Stairs
	auto Stairs = std::move(_token);

	try
	{
		while (input.good() && input.peek() != EOF)
		{
			// Call the appropriate member function 
			// to handle the character
			(this->*ScanChar[input.peek()])(input);
		}
	}
	catch(const std::invalid_argument& e)
	{
		_token = std::move(Stairs);
		throw e;
	}
	
}

bool LexicalAnalyzer::HasToken() const
{
	return _token.size();
}

void LexicalAnalyzer::Next()
{
	_token.pop_front();
}

string LexicalAnalyzer::Get() const
{
	return _token.front();
}


// H E L P E R  F U N C T I O N S

void LexicalAnalyzer::ScanIdentifier(istream& input)
{
	string token;
	for (int iii=0; iii<16 && isalnum(input.peek()); ++iii)
		token += GetChar(input);

	if (token.size() > 15)
		Throw("Identifier longer than 15 characters");

	_token.push_back(token);
}

void LexicalAnalyzer::ScanNumber(std::istream& input)
{
	string token;

	while (isdigit(input.peek()))
		token += GetChar(input);

	_token.push_back(token);
}

void LexicalAnalyzer::ScanCharConst(istream& input)
{
	string token;
	token+=GetChar(input);
	if (!isprint(input.peek()))
		Throw(string("Unprintable Character. Charcode: ")+=to_string(input.peek()));

	token+=GetChar(input);

	if (input.peek()!='\'')
		Throw("Missing closing tick for char constant.");

	token+=GetChar(input);

	_token.push_back(token);
}

void LexicalAnalyzer::ScanSpecial(istream& input)
{
	_token.push_back(string()+=GetChar(input));
}

void LexicalAnalyzer::ScanSpace(istream& input)
{
	// Discards all white space
	while (isspace(input.peek()))
		GetChar(input);
}

void LexicalAnalyzer::ScanUnequal(istream& input)
{
	string token;
	token += GetChar(input);
	if (input.peek()=='=')
		token += GetChar(input);
	else 
		Throw("\'!\' not followed by \'=\'.");

	_token.push_back(token);
}

void LexicalAnalyzer::ScanLeftParenthesis(istream& input)
{
	string token;
	token += GetChar(input);

	if (input.peek() == '.' )
		token += GetChar(input);

	// Extract ( and (. and end
	if (input.peek() != '*')
	{
		_token.push_back(token);	
		return;
	}

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
}

void LexicalAnalyzer::ScanAsterisk(istream& input)
{
	// psssst! Don't tell anyone =)
	ScanDot(input);
}

void LexicalAnalyzer::ScanDot(istream& input)
{
	string token;
	token += GetChar(input);
	if (input.peek()==')')
		token += GetChar(input);
	
	_token.push_back(token);
}

void LexicalAnalyzer::ScanColon(istream& input)
{
	// It'll be a secret between us ;)
	ScanComparison(input);
}

void LexicalAnalyzer::ScanComparison(istream& input)
{
	string token;
	token+=GetChar(input);
	if (input.peek() == '=')
		token+=GetChar(input);

	_token.push_back(token);
}

void LexicalAnalyzer::ScanIllegal(istream& input)
{
	string msg("Illegal char: ");
	msg += input.peek();
	
	Throw(msg);
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
		_col=0;
	}
	else 
		++_col;
	return input.get();
}


vector<decltype(&LexicalAnalyzer::ScanIllegal)> LexicalAnalyzer::ScanChar = decltype(ScanChar)(); 

void LexicalAnalyzer::InitScanChar()
{
	// Only init scan table once
	if (LexicalAnalyzer::ScanChar.size())
		return;

	ScanChar = decltype(ScanChar)(256, &LexicalAnalyzer::ScanIllegal);

	LexicalAnalyzer::ScanChar['.'] = &LexicalAnalyzer::ScanDot;
	LexicalAnalyzer::ScanChar[':'] = &LexicalAnalyzer::ScanColon;
	LexicalAnalyzer::ScanChar['!'] = &LexicalAnalyzer::ScanUnequal;
	LexicalAnalyzer::ScanChar['\'']= &LexicalAnalyzer::ScanCharConst;
	LexicalAnalyzer::ScanChar['>'] = &LexicalAnalyzer::ScanComparison;
	LexicalAnalyzer::ScanChar['<'] = &LexicalAnalyzer::ScanComparison;
	LexicalAnalyzer::ScanChar['('] = &LexicalAnalyzer::ScanLeftParenthesis;

	for (auto c: " \t\n\v\f\r")
		LexicalAnalyzer::ScanChar[c] = &LexicalAnalyzer::ScanSpace;

	for (auto c: "+-/*=,;)")
		LexicalAnalyzer::ScanChar[c] = &LexicalAnalyzer::ScanSpecial;

	for (auto c: "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
		LexicalAnalyzer::ScanChar[c] = &LexicalAnalyzer::ScanIdentifier;

	for (auto c: "0123456789")
		LexicalAnalyzer::ScanChar[c] = &LexicalAnalyzer::ScanNumber;
}