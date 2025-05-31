// /nirvana/prep_ai/../code/src/lexer.cpp
#include "../include/lexer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <stdexcept>

Lexer::Lexer(std::string source) : source(std::move(source)), current(0), line(1) {}

char Lexer::peek()
{
    if (current >= source.length())
    {
        return '\0';
    }
    return source[current];
}

char Lexer::peekNext()
{
    if (current + 1 >= source.length())
    {
        return '\0';
    }
    return source[current + 1];
}

char Lexer::advance()
{
    char c = '\0';
    if (current < source.length())
    {
        c = source[current++];
        if (c == '\n')
        {
            line++;
        }
    }
    return c;
}

void Lexer::skipWhitespace()
{
    while (true)
    {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
        {
            advance();
            continue;
        }

        if (c == '/')
        {
            if (peekNext() == '/')
            {
                advance();
                advance();
                while (peek() != '\n' && peek() != '\0')
                {
                    advance();
                }
                continue;
            }
            else if (peekNext() == '*')
            {
                advance();
                advance();
                while (!(peek() == '*' && peekNext() == '/') && peek() != '\0')
                {
                    advance();
                }
                if (peek() != '\0')
                {
                    advance();
                    advance();
                }
                continue;
            }
        }
        break;
    }
}

Token Lexer::identifierOrKeyword()
{
    std::string lexeme;
    int startLine = line;

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
        return Token(TokenType::ECHO, lexeme, startLine);
    }
    if (lexeme == "true")
    {
        return Token(TokenType::TRUE, lexeme, true, startLine);
    }
    if (lexeme == "false")
    {
        return Token(TokenType::FALSE, lexeme, false, startLine);
    }
    if (lexeme == "and")
    {
        return Token(TokenType::AND, lexeme, startLine);
    }
    if (lexeme == "or")
    {
        return Token(TokenType::OR, lexeme, startLine);
    }
    if (lexeme == "not")
    {
        return Token(TokenType::NOT, lexeme, startLine);
    }

    return Token(TokenType::IDENTIFIER, lexeme, startLine);
}

Token Lexer::string()
{
    int startLine = line;
    advance();
    std::string value;

    while (peek() != '"' && peek() != '\0')
    {
        value += advance();
    }

    if (peek() == '\0')
    {
        std::cerr << "Lexer Error: Unterminated string literal at line " << startLine << ".\n";
        return Token(TokenType::UNKNOWN, "", startLine);
    }

    advance();

    return Token(TokenType::STRING_LITERAL, "\"" + value + "\"", value, startLine);
}

Token Lexer::number()
{
    std::string lexeme;
    int startLine = line;
    while (std::isdigit(peek()))
    {
        lexeme += advance();
    }

    if (peek() == '.' && std::isdigit(peekNext()))
    {
        lexeme += advance();

        while (std::isdigit(peek()))
        {
            lexeme += advance();
        }
    }

    try
    {
        double value = std::stod(lexeme);
        return Token(TokenType::NUMBER_LITERAL, lexeme, value, startLine);
    }
    catch (const std::out_of_range &)
    {
        std::cerr << "Lexer Error: Number literal out of range: " << lexeme << " at line " << startLine << ".\n";
        return Token(TokenType::UNKNOWN, lexeme, startLine);
    }
    catch (const std::invalid_argument &)
    {
        std::cerr << "Lexer Error: Invalid number literal: " << lexeme << " at line " << startLine << ".\n";
        return Token(TokenType::UNKNOWN, lexeme, startLine);
    }
}

Token Lexer::getNextToken()
{
    skipWhitespace();

    char c = peek();
    int startLine = line;

    if (c == '\0')
    {
        return Token(TokenType::EOF_TOKEN, "", startLine);
    }

    switch (c)
    {
    case ';':
        advance();
        return Token(TokenType::SEMICOLON, ";", startLine);
    case '.':
        advance();
        return Token(TokenType::DOT, ".", startLine);
    case '(':
        advance();
        return Token(TokenType::LEFT_PAREN, "(", startLine);
    case ')':
        advance();
        return Token(TokenType::RIGHT_PAREN, ")", startLine);
    case '+':
        advance();
        return Token(TokenType::PLUS, "+", startLine);
    case '-':
        advance();
        return Token(TokenType::MINUS, "-", startLine);
    case '*':
        advance();
        return Token(TokenType::STAR, "*", startLine);
    case '/':
        advance();
        return Token(TokenType::SLASH, "/", startLine);
    case '!':
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(TokenType::BANG_EQUAL, "!=", startLine);
        }
        return Token(TokenType::BANG, "!", startLine);
    }
    case '=':
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(TokenType::EQUAL_EQUAL, "==", startLine);
        }
        return Token(TokenType::EQUAL, "=", startLine);
    }
    case '<':
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(TokenType::LESS_EQUAL, "<=", startLine);
        }
        return Token(TokenType::LESS, "<", startLine);
    }
    case '>':
    {
        advance();
        if (peek() == '=')
        {
            advance();
            return Token(TokenType::GREATER_EQUAL, ">=", startLine);
        }
        return Token(TokenType::GREATER, ">", startLine);
    }
    case '"':
        return string();
    case '$':
        return identifierOrKeyword();
    default:
        if (std::isalpha(c))
        {
            return identifierOrKeyword();
        }
        if (std::isdigit(c))
        {
            return number();
        }

        std::cerr << "Lexer Error: Unexpected character '" << c << "' at line " << startLine << ".\n";
        advance();
        return Token(TokenType::UNKNOWN, std::string(1, c), startLine);
    }
}