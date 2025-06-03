
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <memory>
#include "lexer.h"
#include "ast.h"
#include "token.h"

class Parser
{
private:
    Lexer &lexer;
    Token currentToken;

    void advance();
    void consume(TokenType type);

    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parseCall();
    std::unique_ptr<ASTNode> parseConcatenation();
    std::unique_ptr<ASTNode> parsePrimaryExpression();

public:
    Parser(Lexer &lexer);

    std::unique_ptr<ProgramNode> parseProgram();
};

#endif