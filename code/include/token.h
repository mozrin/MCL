// /nirvana/prep_ai/../code/include/token.h
#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <variant>
#include <utility>
#include <cstdint>

#include "value.h"

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

PUBLIC,
STRING_KEYWORD,
INTEGER_KEYWORD,
NUMBER_KEYWORD,
BOOLEAN_KEYWORD,

EOF_TOKEN,
UNKNOWN,
};

inline DeclaredType tokenTypeToDeclaredType(TokenType type) {
    switch (type) {
        case TokenType::STRING_KEYWORD: return DeclaredType::STRING;
        case TokenType::INTEGER_KEYWORD: return DeclaredType::INTEGER;
        case TokenType::NUMBER_KEYWORD: return DeclaredType::NUMBER;
        case TokenType::BOOLEAN_KEYWORD: return DeclaredType::BOOLEAN;
        default: return DeclaredType::ANY;
    }
}

struct Token
{
TokenType type;
std::string lexeme;
std::variant<std::string, long long, double, bool, std::monostate> literal;
int line;

Token(TokenType type, std::string lexeme, std::variant<std::string, long long, double, bool, std::monostate> literal, int line)
: type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

Token(TokenType type, std::string lexeme, int line)
: type(type), lexeme(std::move(lexeme)), literal(std::monostate{}), line(line) {}

std::string toString() const;
};

#endif