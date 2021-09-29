/*
 * Break input into tokens
 */
#ifndef LEXICAL_ANALYZER_HPP
#define LEXICAL_ANALYZER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <list>

// TODO: Create Token class
class LexicalAnalyzer 
{
public:
	LexicalAnalyzer();

	// Scan a stream of characters
	// Throws std::invalid_argument if a character
	// is not in KPL alphabet
	void Scan(std::istream& input);

	// Return true if there's some scanned token left 
	// from the stream
	bool HasToken() const;

	// Discard current token and move to the next one 
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

		// All kinds of integers 
		void ScanNumber(std::istream& input);

		// 'x' where x is a printable character between two ticks
		void ScanCharConst(std::istream& input);

		// +-/*=,;)
		void ScanSpecial(std::istream& input);

		// Scan and ignore a bunch of characters 
		// where each of them isspace() 
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

		// Anything that's not in other Scan*()
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

