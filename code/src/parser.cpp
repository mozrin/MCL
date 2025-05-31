#include "parser.h"
#include <iostream>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.getNextToken())
{
}

void Parser::advance()
{
    currentToken = lexer.getNextToken();

    if (currentToken.type == TokenType::UNKNOWN)
    {
        std::cerr << "Parser error: Lexer returned UNKNOWN token: \"" << currentToken.lexeme << "\". Aborting.\n";
        exit(1);
    }
}

void Parser::consume(TokenType type)
{
    if (currentToken.type == type)
    {
        advance();
    }
    else
    {

        std::cerr << "Parser error: Expected token type " << Token(type, "").toString()
                  << " but found " << currentToken.toString() << ".\n";
        exit(1);
    }
}

std::unique_ptr<ASTNode> Parser::parsePrimaryExpression()
{
    if (currentToken.type == TokenType::STRING_LITERAL)
    {

        std::string value = std::get<std::string>(currentToken.literal);
        consume(TokenType::STRING_LITERAL);
        return std::make_unique<StringLiteralExpr>(value);
    }
    else if (currentToken.type == TokenType::IDENTIFIER)
    {

        std::string name = currentToken.lexeme;
        consume(TokenType::IDENTIFIER);
        return std::make_unique<VariableExpr>(name);
    }
    else if (currentToken.type == TokenType::LEFT_PAREN)
    {
        consume(TokenType::LEFT_PAREN);
        std::unique_ptr<ASTNode> expr = parseExpression();
        consume(TokenType::RIGHT_PAREN);
        return expr;
    }

    std::cerr << "Parser error: Expected a primary expression (string literal, variable, or parenthesized expression), but got "
              << currentToken.toString() << ".\n";
    exit(1);
}

std::unique_ptr<ASTNode> Parser::parseExpression()
{

    std::unique_ptr<ASTNode> left = parsePrimaryExpression();

    while (currentToken.type == TokenType::DOT)
    {
        TokenType op = currentToken.type;
        consume(TokenType::DOT);
        std::unique_ptr<ASTNode> right = parsePrimaryExpression();

        left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseStatement()
{
    if (currentToken.type == TokenType::ECHO)
    {
        consume(TokenType::ECHO);
        std::unique_ptr<ASTNode> expr = parseExpression();
        consume(TokenType::SEMICOLON);
        return std::make_unique<EchoStatement>(std::move(expr));
    }
    else if (currentToken.type == TokenType::IDENTIFIER)
    {

        std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(currentToken.lexeme);
        consume(TokenType::IDENTIFIER);
        consume(TokenType::EQUAL);
        std::unique_ptr<ASTNode> value_expr = parseExpression();
        consume(TokenType::SEMICOLON);
        return std::make_unique<AssignmentStatement>(std::move(var_target), std::move(value_expr));
    }

    std::cerr << "Parser error: Unexpected token at the beginning of a statement: "
              << currentToken.toString() << ".\n";
    exit(1);
}

std::unique_ptr<ProgramNode> Parser::parseProgram()
{
    auto program = std::make_unique<ProgramNode>();

    while (currentToken.type != TokenType::EOF_TOKEN)
    {
        program->statements.push_back(parseStatement());
    }
    return program;
}