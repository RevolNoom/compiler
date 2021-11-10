CPP = g++
CPPFLAGS = -g -Wall

compiler: Token.o compiler.o LexicalAnalyzer.o Component.o
	$(CPP) $(CPPFLAGS) -o compiler Token.o compiler.o Component.o LexicalAnalyzer.o

compiler.o: compiler.cpp LexicalAnalyzer.hpp
	$(CPP) $(CPPFLAGS) -c compiler.cpp -o compiler.o  

Component.o: Component.cpp Component.hpp
	$(CPP) $(CPPFLAGS) -c Component.cpp -o Component.o  

LexicalAnalyzer.o: LexicalAnalyzer.hpp LexicalAnalyzer.cpp
	$(CPP) $(CPPFLAGS) -c LexicalAnalyzer.cpp -o LexicalAnalyzer.o  

Token.o: Token.cpp Token.hpp
	$(CPP) $(CPPFLAGS) -c Token.cpp -o Token.o  

Token.hpp Token.cpp: GenEverything.bash
	./GenEverything.bash


clean:
	rm -f *.o *~
