#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.hpp"

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cerr<<"Usage: "<<argv[0]<<" FILE1 FILE2 ...\nAnalyze KPL source code and print out number of characters and digits.\n";
		return 1;
	}

	LexicalAnalyzer LA;

	int digit=0;
	int alpha=0;

	for (int iii=1; iii<argc; ++iii)
	{
		std::ifstream example;
		example.open(argv[iii]);

		try 
		{	
			LA.Scan(example);
			while (LA.HasToken())
			{
				std::string token = LA.Get();
				for (auto t: token)
				{
					if (std::isdigit(t))
						++digit;
					if (std::isalpha(t))
						++alpha;
				}
				LA.Next();
			}
		}
		catch (const std::invalid_argument &e)
		{
			std::cerr<<"In file "<<argv[iii]<<": "<<e.what()<<'\n';
		}
	}

	std::cout<<"Without comments, there are totally "<<digit<<" digits and "<<alpha<<" letters in those files\n";

	return 0;
} 
