// include/lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>  // Not strictly needed for Lexer output as a stream, but common
#include "token.h" // To use our Token struct

class Lexer
{
private:
    std::string source; // The entire input Nirvana code string
    int current;        // Current character index in the 'source' string

    // Helper functions for character-level operations:
    char peek();           // Look at the character at 'current' without advancing.
    char advance();        // Consume the character at 'current' and move 'current' forward.
    void skipWhitespace(); // Ignore spaces, tabs, newlines, carriage returns.

    // Functions to recognize specific token types:
    Token identifierOrKeyword(); // Handles sequences starting with '$' or letters (e.g., $name, echo).
    Token string();              // Handles string literals enclosed in double quotes (e.g., "hello").

public:
    // Constructor: Takes the source code string.
    Lexer(std::string source);

    // Main function: Returns the next token from the source code.
    Token getNextToken();
};

#endif // LEXER_H