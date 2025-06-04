#include "lexer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <stdexcept>
#include <map>

static const std::map<std::string, TokenType> keywords = {
    {"echo", TokenType::ECHO},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"public", TokenType::PUBLIC},
    {"string", TokenType::STRING_KEYWORD},
    {"integer", TokenType::INTEGER_KEYWORD},
    {"number", TokenType::NUMBER_KEYWORD},
    {"boolean", TokenType::BOOLEAN_KEYWORD}};

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
        else if (c == '#')
        {
            advance();
            while (peek() != '\n' && peek() != '\0')
            {
                advance();
            }
            continue;
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

    while (std::isalnum(peek()) || peek() == '_' || peek() == '-')
    {
        lexeme += advance();
    }

    if (lexeme.empty() || lexeme[0] != '$')
    {
        auto it = keywords.find(lexeme);
        if (it != keywords.end())
        {
            if (it->second == TokenType::TRUE)
                return Token(TokenType::TRUE, lexeme, true, startLine);
            if (it->second == TokenType::FALSE)
                return Token(TokenType::FALSE, lexeme, false, startLine);
            return Token(it->second, lexeme, startLine);
        }
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
        if (peek() == '\\')
        {
            advance();
            char escaped_char = peek();
            if (escaped_char == '\0')
            {
                throw std::runtime_error("Unterminated escape sequence in string literal at line " + std::to_string(startLine) + ".");
            }

            switch (escaped_char)
            {
            case '"':
                value += '"';
                break;
            case '\\':
                value += '\\';
                break;
            case 'n':
                value += '\n';
                break;
            case 't':
                value += '\t';
                break;
            default:
                value += escaped_char;
                break;
            }
            advance();
        }
        else
        {
            value += advance();
        }
    }

    if (peek() == '\0')
    {
        throw std::runtime_error("Unterminated string literal at line " + std::to_string(startLine) + ".");
    }

    advance();

    return Token(TokenType::STRING_LITERAL, "\"" + value + "\"", value, startLine);
}

Token Lexer::number()
{
    std::string lexeme;
    int startLine = line;
    bool has_decimal = false;

    while (std::isdigit(peek()))
    {
        lexeme += advance();
    }

    if (peek() == '.' && std::isdigit(peekNext()))
    {
        has_decimal = true;
        lexeme += advance();

        while (std::isdigit(peek()))
        {
            lexeme += advance();
        }
    }

    if (has_decimal)
    {
        try
        {
            double value = std::stod(lexeme);
            return Token(TokenType::NUMBER_LITERAL, lexeme, value, startLine);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error("Number literal out of range: " + lexeme + " at line " + std::to_string(startLine) + ".");
        }
        catch (const std::invalid_argument &)
        {
            throw std::runtime_error("Invalid number literal: " + lexeme + " at line " + std::to_string(startLine) + ".");
        }
    }
    else
    {
        try
        {
            long long value = std::stoll(lexeme);
            return Token(TokenType::NUMBER_LITERAL, lexeme, value, startLine);
        }
        catch (const std::out_of_range &)
        {
            throw std::runtime_error("Integer literal out of range: " + lexeme + " at line " + std::to_string(startLine) + ".");
        }
        catch (const std::invalid_argument &)
        {
            throw std::runtime_error("Invalid integer literal: " + lexeme + " at line " + std::to_string(startLine) + ".");
        }
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
    case ',':
        advance();
        return Token(TokenType::COMMA, ",", startLine);
    case '|':
        advance();
        return Token(TokenType::PIPE, "|", startLine);
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

        throw std::runtime_error("Unexpected character '" + std::string(1, c) + "' at line " + std::to_string(startLine) + ".");
    }
}