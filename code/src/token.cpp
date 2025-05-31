#include "../include/token.h"
#include <map>
#include <string>

static const std::map<TokenType, std::string> tokenTypeNames = {
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::DOT, "DOT"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING_LITERAL, "STRING_LITERAL"},
    {TokenType::ECHO, "ECHO"},
    {TokenType::EOF_TOKEN, "EOF_TOKEN"},
    {TokenType::UNKNOWN, "UNKNOWN"},
};

std::string Token::toString() const
{

    std::string typeName = tokenTypeNames.count(type) ? tokenTypeNames.at(type) : "UNKNOWN_TYPE";
    std::string str = typeName + " (\"" + lexeme + "\")";

    if (literal.index() == 0)
    {
        str += " -> \"" + std::get<std::string>(literal) + "\"";
    }
    else if (literal.index() == 1)
    {

        str += std::string(" -> ") + std::to_string(std::get<double>(literal));
    }
    else if (literal.index() == 2)
    {

        str += std::string(" -> ") + (std::get<bool>(literal) ? "true" : "false");
    }

    return str;
}