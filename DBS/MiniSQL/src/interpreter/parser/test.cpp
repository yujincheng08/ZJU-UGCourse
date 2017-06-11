#include <iostream>
#include "parser.h"

using namespace std;

void *ParserAlloc(void *(*)(size_t));
void ParserFree(void *, void(*)(void *));
void Parser(void *, int, const string *);


int main()
{
	void * parser = ParserAlloc(malloc);
	string x;
	Parser(parser, CREATE, &(x="create"));
	Parser(parser, STRING, &(x="x"));
	Parser(parser, INT_TYPE, &(x="int"));
	Parser(parser, RP, &(x="int"));
	Parser(parser, SEMICOLON, &(x=";"));
    //Parser(parser, 0, nullptr);
    Parser(parser, CREATE, &(x="create"));
	Parser(parser, STRING, &(x="x"));
	Parser(parser, INT_TYPE, &(x="int"));
	Parser(parser, RP, &(x="int"));
	Parser(parser, SEMICOLON, &(x=";"));
	ParserFree(parser, free);
	return 0;
}
