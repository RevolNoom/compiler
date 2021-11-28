/*
 * Break input into tokens
 * OOP pattern: Singleton
 */
#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <unordered_map>
#include <iostream>
#include <vector>
#include <string>

#include "Token.hpp"
#include "TokenData.hpp"
#include "TokenSpecial.hpp"
#include "TokenKeyword.hpp"

class LexicalAnalyzer 
{
public:
	static LexicalAnalyzer* singleton();

	// Scan a stream of characters
	// Return a stream of Token
	// If there's an error,
	// throws std::invalid_argument 
	TokenStream Scan(std::istream& input);

private:

	// S C A N   F U N C T I O N S

		// Start with alphabet character
		// Can contain number & alpha
		// Max 15 characters
		// abc123 
		shared_ptr<Token> ScanIdentifier(std::istream& input);

		// Scan POSITIVE integers 
		shared_ptr<Token> ScanNumber(std::istream& input);

		// 'x': a printable character between two ticks
		shared_ptr<Token> ScanCharConst(std::istream& input);

		// +-/*=,;)><:.
		// <=  >=  .)  :=
		shared_ptr<Token> ScanSymbol(std::istream& input);

		// All kind of spaces
		shared_ptr<Token> ScanSpace(std::istream& input);

		// !=
		shared_ptr<Token> ScanUnequal(std::istream& input);

		//	(	or	(.		
		//	If it's (* then discard the whole comment upto *)
		shared_ptr<Token> ScanLeftParenthesis(std::istream& input);

		// Anything not in other Scan functions
		shared_ptr<Token> ScanIllegal(std::istream& input);


	// M I S C E L L A N E O U S

		// Throws invalid_argument with msg descriptions of what went wrong
		// Informations include: What (msg) and where went wrong
		void Throw(std::string msg);

		// Get and keep track of current position in the stream
		// Use this instead of input.get()
		int GetChar(std::istream& input);
private:
	LexicalAnalyzer();
	static LexicalAnalyzer *_singleton;
	
private:
	// Current position in the stream
	int _line, _col;

	// MAPPINGS

	// A mapping between a char and its corresponding scan function
	vector<decltype(&LexicalAnalyzer::ScanIllegal)> _scanChar;

	// Keywords - Token mapping
	unordered_map<string, shared_ptr<Token>> _keyword;

};

#endif

