#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.hpp"
#include "Component.hpp"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr<<"parse FILE1.kpl FILE2.kpl ...\nParses KPL source codes.";
		return 1;
	}

	for (int iii=1; iii<argc; ++iii)
	{
		std::ifstream file(argv[iii]);
		
		if (!file.is_open())
		{
			std::cerr<<"Can't open file "<<argv[iii]<<"\n";
			continue;
		}

		try
		{
			LexicalAnalyzer* LA = LexicalAnalyzer::Singleton();
			auto tokenstream = LA->Scan(file);
			Component::Program p;
			p.Parse(tokenstream);
		}
		catch (const std::exception &e)
		{
			std::cout<<e.what()<<"\n";
		}
		
	}
	return 0;
} 
