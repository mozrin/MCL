// /nirvana/prep_ai/../code/include/lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer
{
private:
std::string source;
int current;
int line;

char peek();
char advance();
char peekNext();
void skipWhitespace();

Token identifierOrKeyword();
Token string();
Token number();

public:
Lexer(std::string source);
Token getNextToken();
};

#endif