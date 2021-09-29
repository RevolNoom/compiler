CPPFLAGS = -std=c++17 
CPP = g++

all: scanner

scanner: scanner.o LexicalAnalyzer.o 
	${CPP} scanner.o LexicalAnalyzer.o -o scanner

scanner.o:

LexicalAnalyzer.o: LexicalAnalyzer.hpp

clean:
	rm -f *.o *~

