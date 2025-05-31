// include/parser.h
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>  // For std::unique_ptr
#include "lexer.h" // To interact with the Lexer
#include "ast.h"   // To construct AST nodes
#include "token.h" // To use Token and TokenType

class Parser
{
private:
    Lexer &lexer;       // Reference to our Lexer instance to get tokens from
    Token currentToken; // The token currently being processed by the parser

    // Helper functions for token consumption and error handling:
    void advance();               // Consume the currentToken and load the next token from the lexer.
    void consume(TokenType type); // Consume currentToken and check if its type matches expected.
                                  // Throws an error if types don't match.

    // Recursive Descent Parsing functions:
    // These functions correspond to grammar rules and build specific parts of the AST.
    std::unique_ptr<ASTNode> parseStatement();         // Parses a single statement (e.g., assignment, echo).
    std::unique_ptr<ASTNode> parseExpression();        // Parses a general expression (e.g., concatenation).
    std::unique_ptr<ASTNode> parsePrimaryExpression(); // Parses the simplest expressions (literals, variables).

public:
    // Constructor: Takes a reference to the Lexer.
    Parser(Lexer &lexer);

    // Main parsing function: Parses the entire source code and returns the root ProgramNode of the AST.
    std::unique_ptr<ProgramNode> parseProgram();
};

#endif // PARSER_H