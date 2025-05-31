// /nirvana/prep_ai/../code/src/token.cpp
#include "../include/token.h"
#include <map>
#include <string>
#include <iomanip>
#include <sstream>

static const std::map<TokenType, std::string> tokenTypeNames = {
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::DOT, "DOT"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::STAR, "STAR"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::BANG, "BANG"},
    {TokenType::LESS, "LESS"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::NUMBER_LITERAL, "NUMBER_LITERAL"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::ECHO, "ECHO"},
    {TokenType::AND, "AND"},
    {TokenType::OR, "OR"},
    {TokenType::NOT, "NOT"},
    {TokenType::EOF_TOKEN, "EOF_TOKEN"},
    {TokenType::UNKNOWN, "UNKNOWN"},
};

std::string Token::toString() const
{
    std::string typeName = tokenTypeNames.count(type) ? tokenTypeNames.at(type) : "UNKNOWN_TYPE";
    std::string str = typeName + " (\"" + lexeme + "\")";

    if (std::holds_alternative<std::string>(literal))
    {
        str += " -> \"" + std::get<std::string>(literal) + "\"";
    }
    else if (std::holds_alternative<double>(literal))
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(5) << std::get<double>(literal);
        str += " -> " + ss.str();
    }
    else if (std::holds_alternative<bool>(literal))
    {
        str += std::string(" -> ") + (std::get<bool>(literal) ? "true" : "false");
    }

    str += " at line " + std::to_string(line);

    return str;
}