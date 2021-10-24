#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.hpp"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cerr<<"Usage: "<<argv[0]<<" FILE1 FILE2 ...\nAnalyze KPL source code and print out tokens & their types.\n";
		return 1;
	}

	for (int iii=1; iii<argc; ++iii)
	{
		std::ifstream example;
		example.open(argv[iii]);

		try 
		{	
			std::cout<<"All tokens in file "<<argv[iii]<<":\n";
			auto tokens = LexicalAnalyzer::singleton()->Scan(example);

			for (auto t: tokens)
			{
				std::cout<<t->GetTokenType()<<": "<<t->ToString()<<"\n";
			}
		}
		catch (const std::invalid_argument &e)
		{
			std::cerr<<"In file "<<argv[iii]<<": "<<e.what()<<'\n';
		}
	}

	return 0;
} 
