// /nirvana/prep_ai/../code/include/token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <utility>

enum class TokenType
{
    SEMICOLON,
    DOT,
    EQUAL,
    LEFT_PAREN,
    RIGHT_PAREN,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    BANG,
    LESS,
    GREATER,

    BANG_EQUAL,
    EQUAL_EQUAL,
    LESS_EQUAL,
    GREATER_EQUAL,

    IDENTIFIER,
    STRING_LITERAL,
    NUMBER_LITERAL,
    TRUE,
    FALSE,

    ECHO,
    AND,
    OR,
    NOT,

    EOF_TOKEN,
    UNKNOWN,
};

struct Token
{
    TokenType type;
    std::string lexeme;
    std::variant<std::string, double, bool, std::monostate> literal;
    int line;

    Token(TokenType type, std::string lexeme, std::variant<std::string, double, bool, std::monostate> literal, int line)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    Token(TokenType type, std::string lexeme, int line)
        : type(type), lexeme(std::move(lexeme)), literal(std::monostate{}), line(line) {}

    std::string toString() const;
};

#endif