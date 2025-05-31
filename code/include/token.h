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

    IDENTIFIER,
    STRING_LITERAL,

    ECHO,

    EOF_TOKEN,
    UNKNOWN,
};

struct Token
{
    TokenType type;
    std::string lexeme;

    std::variant<std::string, double, bool, std::monostate> literal;

    Token(TokenType type, std::string lexeme, std::variant<std::string, double, bool, std::monostate> literal)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)) {}

    Token(TokenType type, std::string lexeme)
        : type(type), lexeme(std::move(lexeme)), literal(std::monostate{}) {}

    std::string toString() const;
};

#endif