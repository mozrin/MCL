#include "uppercase.h"
#include "../../core/constants.h"
#include <cmath>
#include <stdexcept>
#include <variant>
#include <string>
#include <cctype>
#include <sstream>

char get_toupper(char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); }
char get_tolower(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }
bool is_alpha(char c) { return static_cast<bool>(std::isalpha(static_cast<unsigned char>(c))); }
bool is_space(char c) { return static_cast<bool>(std::isspace(static_cast<unsigned char>(c))); }
bool is_alnum(char c) { return static_cast<bool>(std::isalnum(static_cast<unsigned char>(c))); }
bool is_lower(char c) { return static_cast<bool>(std::islower(static_cast<unsigned char>(c))); }
bool is_upper(char c) { return static_cast<bool>(std::isupper(static_cast<unsigned char>(c))); }

Value mcl_uppercase(const std::vector<Value> &args)
{
    if (args.empty() || args.size() > 2)
    {
        throw std::runtime_error("Function 'uppercase' expects 1 or 2 arguments: uppercase(string $value, int $technique=MCL_UPPERCASE_EVERYTHING).");
    }

    std::string input_string;
    if (std::holds_alternative<std::string>(args[0]))
    {
        input_string = std::get<std::string>(args[0]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'uppercase': Argument 1 ($value) must be a string, but got value of type ";
        switch (valueTypeToDeclaredType(args[0]))
        {
        case DeclaredType::INTEGER:
            ss << "integer";
            break;
        case DeclaredType::NUMBER:
            ss << "number";
            break;
        case DeclaredType::BOOLEAN:
            ss << "boolean";
            break;
        default:
            ss << "unknown";
            break;
        }
        ss << ".";
        throw std::runtime_error(ss.str());
    }

    long long technique = MCL_UPPERCASE_EVERYTHING;
    if (args.size() == 2)
    {
        if (std::holds_alternative<long long>(args[1]))
        {
            technique = std::get<long long>(args[1]);
        }
        else if (std::holds_alternative<double>(args[1]))
        {
            double d_tech = std::get<double>(args[1]);
            if (d_tech != std::floor(d_tech))
            {
                std::stringstream ss;
                ss << "Function 'uppercase': Argument 2 ($technique) must be an integer, but got " << d_tech << ".";
                throw std::runtime_error(ss.str());
            }
            technique = static_cast<long long>(d_tech);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'uppercase': Argument 2 ($technique) must be an integer (integer or number), but got value of type ";
            switch (valueTypeToDeclaredType(args[1]))
            {
            case DeclaredType::STRING:
                ss << "string";
                break;
            case DeclaredType::BOOLEAN:
                ss << "boolean";
                break;
            default:
                ss << "unknown";
                break;
            }
            ss << ".";
            throw std::runtime_error(ss.str());
        }

        if (technique < MCL_UPPERCASE_EVERYTHING || technique > MCL_UPPERCASE_TOGGLE)
        {
            std::stringstream ss;
            ss << "Function 'uppercase': Invalid technique constant " << technique << ".";
            throw std::runtime_error(ss.str());
        }
    }

    std::string result_string;
    result_string.reserve(input_string.length());

    switch (technique)
    {
    case MCL_UPPERCASE_EVERYTHING:
    {
        for (char c : input_string)
        {
            result_string += get_toupper(c);
        }
        break;
    }
    case MCL_UPPERCASE_TITLE:
    {
        bool capitalize_next = true;
        for (char c : input_string)
        {
            if (is_alpha(c))
            {
                if (capitalize_next)
                {
                    result_string += get_toupper(c);
                    capitalize_next = false;
                }
                else
                {
                    result_string += get_tolower(c);
                }
            }
            else
            {
                result_string += c;
                capitalize_next = true;
            }
        }
        break;
    }
    case MCL_UPPERCASE_FIRST:
    {
        bool first_char_capitalized = false;
        for (char c : input_string)
        {
            if (!first_char_capitalized && is_alpha(c))
            {
                result_string += get_toupper(c);
                first_char_capitalized = true;
            }
            else
            {
                result_string += c;
            }
        }
        break;
    }
    case MCL_UPPERCASE_ALTERNATING:
    {
        bool to_upper = true;
        for (char c : input_string)
        {
            if (is_alpha(c))
            {
                if (to_upper)
                {
                    result_string += get_toupper(c);
                }
                else
                {
                    result_string += get_tolower(c);
                }
                to_upper = !to_upper;
            }
            else
            {
                result_string += c;
            }
        }
        break;
    }
    case MCL_UPPERCASE_TOGGLE:
    {
        for (char c : input_string)
        {
            if (is_lower(c))
            {
                result_string += get_toupper(c);
            }
            else if (is_upper(c))
            {
                result_string += get_tolower(c);
            }
            else
            {
                result_string += c;
            }
        }
        break;
    }
    default:
    {
        throw std::runtime_error("Function 'uppercase': Unhandled technique constant. This is an internal error.");
    }
    }

    return result_string;
}

void register_uppercase_extension(Evaluator &eval)
{
    eval.registerNativeFunction("uppercase", mcl_uppercase);
}