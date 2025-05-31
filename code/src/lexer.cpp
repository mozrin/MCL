#include "../include/lexer.h"
#include <iostream>
#include <cctype>

Lexer::Lexer(std::string source) : source(std::move(source)), current(0) {}

char Lexer::peek()
{
    if (current >= source.length())
    {
        return '\0';
    }
    return source[current];
}

char Lexer::advance()
{
    if (current < source.length())
    {
        return source[current++];
    }
    return '\0';
}

void Lexer::skipWhitespace()
{
    while (peek() == ' ' || peek() == '\t' || peek() == '\n' || peek() == '\r')
    {
        advance();
    }
}

Token Lexer::identifierOrKeyword()
{
    std::string lexeme;

    if (peek() == '$')
    {
        lexeme += advance();
    }

    while (std::isalnum(peek()) || peek() == '_')
    {
        lexeme += advance();
    }

    if (lexeme == "echo")
    {
        return Token(TokenType::ECHO, lexeme);
    }

    return Token(TokenType::IDENTIFIER, lexeme);
}

Token Lexer::string()
{
    advance();
    std::string value;

    while (peek() != '"' && peek() != '\0')
    {
        value += advance();
    }

    if (peek() == '\0')
    {
        std::cerr << "Lexer Error: Unterminated string literal at position " << current << ".\n";

        return Token(TokenType::UNKNOWN, "");
    }

    advance();

    return Token(TokenType::STRING_LITERAL, "\"" + value + "\"", value);
}

Token Lexer::getNextToken()
{
    skipWhitespace();

    char c = peek();

    if (c == '\0')
    {
        return Token(TokenType::EOF_TOKEN, "");
    }

    switch (c)
    {
    case ';':
        advance();
        return Token(TokenType::SEMICOLON, ";");
    case '.':
        advance();
        return Token(TokenType::DOT, ".");
    case '=':
        advance();
        return Token(TokenType::EQUAL, "=");
    case '(':
        advance();
        return Token(TokenType::LEFT_PAREN, "(");
    case ')':
        advance();
        return Token(TokenType::RIGHT_PAREN, ")");
    case '"':
        return string();
    case '$':
        return identifierOrKeyword();
    default:

        if (std::isalpha(c))
        {
            return identifierOrKeyword();
        }

        std::cerr << "Lexer Error: Unexpected character '" << c << "' at position " << current << ".\n";
        advance();
        return Token(TokenType::UNKNOWN, std::string(1, c));
    }
}