// /nirvana/prep_ai/../code/src/parser.cpp
#include "parser.h"
#include <iostream>
#include <string>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.getNextToken())
{
}

void Parser::advance()
{
currentToken = lexer.getNextToken();

if (currentToken.type == TokenType::UNKNOWN)
{
std::cerr << "Parser error: Lexer returned UNKNOWN token: \"" << currentToken.lexeme << "\" at line " << currentToken.line << ". Aborting.\n";
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
std::cerr << "Parser error: Expected token type " << Token(type, "", 0).toString()
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
else if (currentToken.type == TokenType::NUMBER_LITERAL)
{
double value;
if (std::holds_alternative<long long>(currentToken.literal)) {
    value = static_cast<double>(std::get<long long>(currentToken.literal));
} else {
    value = std::get<double>(currentToken.literal);
}
consume(TokenType::NUMBER_LITERAL);
return std::make_unique<NumberLiteralExpr>(value);
}
else if (currentToken.type == TokenType::TRUE)
{
bool value = std::get<bool>(currentToken.literal);
consume(TokenType::TRUE);
return std::make_unique<BooleanLiteralExpr>(value);
}
else if (currentToken.type == TokenType::FALSE)
{
bool value = std::get<bool>(currentToken.literal);
consume(TokenType::FALSE);
return std::make_unique<BooleanLiteralExpr>(value);
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

std::cerr << "Parser error at line " << currentToken.line << ": Expected a primary expression (literal, variable, or parenthesized expression), but got "
<< currentToken.toString() << ".\n";
exit(1);
}

std::unique_ptr<ASTNode> Parser::parseUnary()
{
if (currentToken.type == TokenType::BANG ||
currentToken.type == TokenType::MINUS ||
currentToken.type == TokenType::NOT)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseUnary();
return std::make_unique<UnaryOpExpr>(op, std::move(right));
}
return parsePrimaryExpression();
}

std::unique_ptr<ASTNode> Parser::parseFactor()
{
std::unique_ptr<ASTNode> left = parseUnary();

while (currentToken.type == TokenType::STAR || currentToken.type == TokenType::SLASH)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseUnary();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseTerm()
{
std::unique_ptr<ASTNode> left = parseFactor();

while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseFactor();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseComparison()
{
std::unique_ptr<ASTNode> left = parseTerm();

while (currentToken.type == TokenType::GREATER || currentToken.type == TokenType::GREATER_EQUAL ||
currentToken.type == TokenType::LESS || currentToken.type == TokenType::LESS_EQUAL)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseTerm();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseEquality()
{
std::unique_ptr<ASTNode> left = parseComparison();

while (currentToken.type == TokenType::BANG_EQUAL || currentToken.type == TokenType::EQUAL_EQUAL)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseComparison();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd()
{
std::unique_ptr<ASTNode> left = parseEquality();

while (currentToken.type == TokenType::AND)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseEquality();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalOr()
{
std::unique_ptr<ASTNode> left = parseLogicalAnd();

while (currentToken.type == TokenType::OR)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseLogicalAnd();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseConcatenation()
{
std::unique_ptr<ASTNode> left = parseLogicalOr();

while (currentToken.type == TokenType::DOT)
{
TokenType op = currentToken.type;
advance();
std::unique_ptr<ASTNode> right = parseLogicalOr();
left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
}
return left;
}

std::unique_ptr<ASTNode> Parser::parseExpression()
{
return parseConcatenation();
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
else if (currentToken.type == TokenType::PUBLIC)
{
    consume(TokenType::PUBLIC);
    TokenType declared_type_token = currentToken.type;

    if (declared_type_token != TokenType::STRING_KEYWORD &&
        declared_type_token != TokenType::INTEGER_KEYWORD &&
        declared_type_token != TokenType::NUMBER_KEYWORD &&
        declared_type_token != TokenType::BOOLEAN_KEYWORD)
    {
        std::cerr << "Parser error at line " << currentToken.line << ": Expected a type keyword (string, integer, number, boolean) after 'public', but got "
                  << currentToken.toString() << ".\n";
        exit(1);
    }
    consume(declared_type_token);

    std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(currentToken.lexeme);
    consume(TokenType::IDENTIFIER);

    std::unique_ptr<ASTNode> value_expr = nullptr;
    if (currentToken.type == TokenType::EQUAL)
    {
        consume(TokenType::EQUAL);
        value_expr = parseExpression();
    }
    
    consume(TokenType::SEMICOLON);
    return std::make_unique<DeclarationStatement>(declared_type_token, std::move(var_target), std::move(value_expr));
}
else if (currentToken.type == TokenType::IDENTIFIER)
{
    std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(currentToken.lexeme);
    consume(TokenType::IDENTIFIER);
    consume(TokenType::EQUAL);
    std::unique_ptr<ASTNode> value_expr = parseExpression();
    consume(TokenType::SEMICOLON);
    return std::make_unique<AssignmentStatement>(std::move(var_target), std::move(value_expr), false);
}

std::cerr << "Parser error at line " << currentToken.line << ": Unexpected token at the beginning of a statement: "
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