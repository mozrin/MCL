#include "parser.h"
#include <iostream>
#include <string>
#include <stdexcept>

Parser::Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.getNextToken())
{
}

void Parser::advance()
{
    currentToken = lexer.getNextToken();

    if (currentToken.type == TokenType::UNKNOWN)
    {
        throw std::runtime_error("Lexer returned UNKNOWN token: \"" + currentToken.lexeme + "\" at line " + std::to_string(currentToken.line) + ".");
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
        throw std::runtime_error("Expected token type " + Token(type, "", 0).toString() + " but found " + currentToken.toString() + ".");
    }
}

TokenType Parser::consumeTypeKeyword()
{
    TokenType type = currentToken.type;
    if (type == TokenType::STRING_KEYWORD ||
        type == TokenType::INTEGER_KEYWORD ||
        type == TokenType::NUMBER_KEYWORD ||
        type == TokenType::BOOLEAN_KEYWORD)
    {
        advance();
        return type;
    }
    throw std::runtime_error("Expected a type keyword (string, integer, number, boolean), but got " +
                             currentToken.toString() + " at line " + std::to_string(currentToken.line) + ".");
}

std::string Parser::consumeIdentifier()
{
    if (currentToken.type == TokenType::IDENTIFIER)
    {
        std::string name = currentToken.lexeme;
        advance();
        return name;
    }
    throw std::runtime_error("Expected an identifier, but got " +
                             currentToken.toString() + " at line " + std::to_string(currentToken.line) + ".");
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
        if (std::holds_alternative<long long>(currentToken.literal))
        {
            value = static_cast<double>(std::get<long long>(currentToken.literal));
        }
        else
        {
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

    throw std::runtime_error("Expected a primary expression (literal, variable, or parenthesized expression), but got " +
                             currentToken.toString() + " at line " + std::to_string(currentToken.line) + ".");
}

std::unique_ptr<ASTNode> Parser::parseCall()
{
    std::unique_ptr<ASTNode> expr = parsePrimaryExpression();

    while (currentToken.type == TokenType::LEFT_PAREN)
    {
        consume(TokenType::LEFT_PAREN);
        std::vector<std::unique_ptr<ASTNode>> args;
        if (currentToken.type != TokenType::RIGHT_PAREN)
        {
            do
            {
                args.push_back(parseExpression());
                if (currentToken.type == TokenType::COMMA)
                {
                    consume(TokenType::COMMA);
                }
                else
                {
                    break;
                }
            } while (true);
        }
        consume(TokenType::RIGHT_PAREN);
        expr = std::make_unique<CallExpr>(std::move(expr), std::move(args));
    }
    return expr;
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
    return parseCall();
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

std::unique_ptr<ASTNode> Parser::parseBitwiseOr()
{
    std::unique_ptr<ASTNode> left = parseEquality();

    while (currentToken.type == TokenType::PIPE)
    {
        TokenType op = currentToken.type;
        advance();
        std::unique_ptr<ASTNode> right = parseEquality();
        left = std::make_unique<BinaryOpExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTNode> Parser::parseLogicalAnd()
{
    std::unique_ptr<ASTNode> left = parseBitwiseOr();

    while (currentToken.type == TokenType::AND)
    {
        TokenType op = currentToken.type;
        advance();
        std::unique_ptr<ASTNode> right = parseBitwiseOr();
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

std::unique_ptr<EchoStatement> Parser::parseEchoStatement()
{
    consume(TokenType::ECHO);
    std::unique_ptr<ASTNode> expr = parseExpression();
    consume(TokenType::SEMICOLON);
    return std::make_unique<EchoStatement>(std::move(expr));
}

std::unique_ptr<DeclarationStatement> Parser::parsePublicDeclarationStatement()
{
    consume(TokenType::PUBLIC);
    TokenType declared_type_token = consumeTypeKeyword();
    std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(consumeIdentifier());

    std::unique_ptr<ASTNode> value_expr = nullptr;
    if (currentToken.type == TokenType::EQUAL)
    {
        consume(TokenType::EQUAL);
        value_expr = parseExpression();
    }
    consume(TokenType::SEMICOLON);
    return std::make_unique<DeclarationStatement>(declared_type_token, std::move(var_target), std::move(value_expr));
}

std::unique_ptr<DeclarationStatement> Parser::parseLocalDeclarationStatement()
{
    TokenType declared_type_token = consumeTypeKeyword();
    std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(consumeIdentifier());

    std::unique_ptr<ASTNode> value_expr = nullptr;
    if (currentToken.type == TokenType::EQUAL)
    {
        consume(TokenType::EQUAL);
        value_expr = parseExpression();
    }
    consume(TokenType::SEMICOLON);
    return std::make_unique<DeclarationStatement>(declared_type_token, std::move(var_target), std::move(value_expr));
}

std::unique_ptr<ReturnStatement> Parser::parseReturnStatement()
{
    consume(TokenType::RETURN);
    std::unique_ptr<ASTNode> expr = nullptr;
    if (currentToken.type != TokenType::SEMICOLON)
    {
        expr = parseExpression();
    }
    consume(TokenType::SEMICOLON);
    return std::make_unique<ReturnStatement>(std::move(expr));
}

std::unique_ptr<BlockStatement> Parser::parseBlock()
{
    auto block = std::make_unique<BlockStatement>();
    consume(TokenType::LEFT_BRACE);
    while (currentToken.type != TokenType::RIGHT_BRACE && currentToken.type != TokenType::EOF_TOKEN)
    {
        block->statements.push_back(parseStatement());
    }
    consume(TokenType::RIGHT_BRACE);
    return block;
}

std::vector<ParameterDeclaration> Parser::parseParameterList()
{
    std::vector<ParameterDeclaration> parameters;
    consume(TokenType::LEFT_PAREN);

    if (currentToken.type != TokenType::RIGHT_PAREN)
    {
        do
        {
            TokenType param_type = consumeTypeKeyword();
            std::string param_name = consumeIdentifier();
            std::unique_ptr<ASTNode> default_value = nullptr;

            if (currentToken.type == TokenType::EQUAL)
            {
                consume(TokenType::EQUAL);
                default_value = parseExpression();
            }
            parameters.emplace_back(param_type, param_name, std::move(default_value));

            if (currentToken.type == TokenType::COMMA)
            {
                consume(TokenType::COMMA);
            }
            else
            {
                break;
            }
        } while (true);
    }
    consume(TokenType::RIGHT_PAREN);
    return parameters;
}

std::unique_ptr<FunctionDeclaration> Parser::parseFunctionDeclaration()
{
    consume(TokenType::FUNCTION);
    std::string func_name = consumeIdentifier();
    std::vector<ParameterDeclaration> parameters = parseParameterList();
    consume(TokenType::COLON);
    TokenType return_type = consumeTypeKeyword();
    std::unique_ptr<BlockStatement> body = parseBlock();

    return std::make_unique<FunctionDeclaration>(func_name, std::move(parameters), return_type, std::move(body));
}

std::unique_ptr<ASTNode> Parser::parseStatement()
{
    if (currentToken.type == TokenType::ECHO)
    {
        return parseEchoStatement();
    }
    else if (currentToken.type == TokenType::PUBLIC)
    {
        return parsePublicDeclarationStatement();
    }
    else if (currentToken.type == TokenType::STRING_KEYWORD ||
             currentToken.type == TokenType::INTEGER_KEYWORD ||
             currentToken.type == TokenType::NUMBER_KEYWORD ||
             currentToken.type == TokenType::BOOLEAN_KEYWORD)
    {
        return parseLocalDeclarationStatement();
    }
    else if (currentToken.type == TokenType::RETURN)
    {
        return parseReturnStatement();
    }
    else if (currentToken.type == TokenType::IDENTIFIER)
    {
        std::string identifier_lexeme = currentToken.lexeme;
        int identifier_line = currentToken.line;
        advance();

        if (currentToken.type == TokenType::EQUAL)
        {
            std::unique_ptr<VariableExpr> var_target = std::make_unique<VariableExpr>(identifier_lexeme);
            consume(TokenType::EQUAL);
            std::unique_ptr<ASTNode> value_expr = parseExpression();
            consume(TokenType::SEMICOLON);
            return std::make_unique<AssignmentStatement>(std::move(var_target), std::move(value_expr), false);
        }
        else if (currentToken.type == TokenType::LEFT_PAREN)
        {
            std::unique_ptr<VariableExpr> callee_expr = std::make_unique<VariableExpr>(identifier_lexeme);

            consume(TokenType::LEFT_PAREN);
            std::vector<std::unique_ptr<ASTNode>> args;
            if (currentToken.type != TokenType::RIGHT_PAREN)
            {
                do
                {
                    args.push_back(parseExpression());
                    if (currentToken.type == TokenType::COMMA)
                    {
                        consume(TokenType::COMMA);
                    }
                    else
                    {
                        break;
                    }
                } while (true);
            }
            consume(TokenType::RIGHT_PAREN);
            consume(TokenType::SEMICOLON);
            return std::make_unique<CallExpr>(std::move(callee_expr), std::move(args));
        }
        else
        {
            throw std::runtime_error("Unexpected token after identifier '" + identifier_lexeme + "'. Expected '=' or '(', but got " +
                                     currentToken.toString() + " at line " + std::to_string(identifier_line) + ".");
        }
    }

    throw std::runtime_error("Unexpected token at the beginning of a statement: " +
                             currentToken.toString() + " at line " + std::to_string(currentToken.line) + ".");
}

std::unique_ptr<ASTNode> Parser::parseTopLevelStatement()
{
    if (currentToken.type == TokenType::FUNCTION)
    {
        return parseFunctionDeclaration();
    }
    else
    {
        return parseStatement();
    }
}

std::unique_ptr<ProgramNode> Parser::parseProgram()
{
    auto program = std::make_unique<ProgramNode>();

    while (currentToken.type != TokenType::EOF_TOKEN)
    {
        program->statements.push_back(parseTopLevelStatement());
    }
    return program;
}