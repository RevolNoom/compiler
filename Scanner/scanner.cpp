/*
 *  file code C viết chán lắm cô ơi 
 *  Em phải viết lại vì em không 
 *  nuốt trôi được cách viết cẩu thả đấy
 * 
 *	main() ở dưới cùng ạ
 *  vvv               vvv
 *     vvv         vvv
 *        vvv   vvv
 *           v v
 *            v
 */




/*
 * Break input into tokens
 */
#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <algorithm>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

using namespace std;

// TODO: Create Token class
class LexicalAnalyzer 
{
public:
	LexicalAnalyzer();

	// Scan a stream of characters
	// If there's an error,
	// throws std::invalid_argument 
	void Scan(std::istream& input);

	// Return true if there's some scanned tokens left 
	bool HasToken() const;

	// Discard current token and move on to the next one 
	void Next();

	// Get the value of current token
	std::string Get() const;


private:

	// S C A N   F U N C T I O N S

		// Start with alphabet character
		// Can contain number & alpha
		// Max 15 characters
		// abc123 
		void ScanIdentifier(std::istream& input);

		// Scan POSITIVE integers 
		void ScanNumber(std::istream& input);

		// 'x': a printable character between two ticks
		void ScanCharConst(std::istream& input);

		// +-/*=,;)
		void ScanSpecial(std::istream& input);

		// Ignore a bunch of isspace() characters 
		void ScanSpace(std::istream& input);

		// !=
		void ScanUnequal(std::istream& input);

		//	(	or	(.		
		//	If it's (* then discard the whole comment
		void ScanLeftParenthesis(std::istream& input);

		// . or .)
		void ScanDot(std::istream& input);

		// * or *)
		void ScanAsterisk(std::istream& input);

		// : or :=
		void ScanColon(std::istream& input);

		// <, >, <=, >=
		void ScanComparison(std::istream& input); 

		// Anything not in other Scan functions
		void ScanIllegal(std::istream& input);


	// M I S C E L L A N E O U S

		// Throws invalid_argument with msg descriptions of what went wrong
		// Informations include: What (msg) and where went wrong
		void Throw(std::string msg);

		// Get and keep track of current position in the stream
		// Use this instead of input.get()
		int GetChar(std::istream& input);
private:
	std::list<std::string> _token;

	// Current position in the stream
	int _line, _col;

	// A mapping between a char and its corresponding scan function
	static std::vector<decltype(&LexicalAnalyzer::ScanIllegal)> ScanChar;

	// Build ScanChar
	static void InitScanChar();
};
#endif



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
	auto StairCol= _col;
	auto StairLine= _line;

	// Restart counters for this new stream
	_col = 0;
	_line = 0;
	
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
		// Return the analyzer to initial state
		_token = std::move(Stairs);
		_col = StairCol;
		_line = StairLine;
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




/*
 * 		MAIN
 *
*/


int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cerr<<"Usage: "<<argv[0]<<" FILE1 FILE2 ...\nAnalyze KPL source code and print out number of characters and digits.\n";
		return 1;
	}

	LexicalAnalyzer LA;

	for (int iii=1; iii<argc; ++iii)
	{
		std::ifstream example;
		example.open(argv[iii]);

		try 
		{	
			LA.Scan(example);
			std::cout<<"All tokens in file "<<argv[iii]<<":\n";
			while (LA.HasToken())
			{
				std::cout<<LA.Get()<<"\n";
				LA.Next();
			}
		}
		catch (const std::invalid_argument &e)
		{
			std::cerr<<"In file "<<argv[iii]<<": "<<e.what()<<'\n';
		}
	}

	return 0;
} 
