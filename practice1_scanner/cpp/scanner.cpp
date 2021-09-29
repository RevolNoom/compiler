#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.hpp"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cerr<<"Usage: "<<argv[0]<<" FILE1 FILE2 ...\nAnalyze KPL source code and print out tokens.\n";
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
			while (LA.HasToken())
			{
				std::cout<<LA.Get()<<'\n';
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
