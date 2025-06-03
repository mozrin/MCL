#include "trim.h"
#include "../../core/constants.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>

Value nirvana_trim(const std::vector<Value> &args)
{
    if (args.empty() || args.size() > 3)
    {
        throw std::runtime_error("Function 'trim' expects 1 to 3 arguments: trim(string $str, string $characters=\" \\t\\n\\r\\0\\x0B\", int $operation=MCL_TRIM_ENDS).");
    }

    std::string input_str;
    if (std::holds_alternative<std::string>(args[0]))
    {
        input_str = std::get<std::string>(args[0]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'trim': Argument 1 ($str) must be a string, but got value of type ";
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

    std::string characters_to_trim = " \t\n\r\0\x0B";
    if (args.size() >= 2)
    {
        if (std::holds_alternative<std::string>(args[1]))
        {
            characters_to_trim = std::get<std::string>(args[1]);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'trim': Argument 2 ($characters) must be a string, but got value of type ";
            switch (valueTypeToDeclaredType(args[1]))
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
    }

    long long operation = MCL_TRIM_ENDS;
    if (args.size() == 3)
    {
        if (std::holds_alternative<long long>(args[2]))
        {
            operation = std::get<long long>(args[2]);
        }
        else if (std::holds_alternative<double>(args[2]))
        {
            double d_op = std::get<double>(args[2]);
            if (d_op != std::floor(d_op))
            {
                std::stringstream ss;
                ss << "Function 'trim': Argument 3 ($operation) must be an integer, but got " << d_op << ".";
                throw std::runtime_error(ss.str());
            }
            operation = static_cast<long long>(d_op);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'trim': Argument 3 ($operation) must be an integer (integer or number), but got value of type ";
            switch (valueTypeToDeclaredType(args[2]))
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

        if (operation < 0 || (operation & ~MCL_TRIM_ALL) != 0)
        {
            std::stringstream ss;
            ss << "Function 'trim': Invalid operation constant " << operation << ". Valid values are combinations of MCL_TRIM_LEFT (1), MCL_TRIM_RIGHT (2), MCL_TRIM_MIDDLE (4), MCL_TRIM_ENDS (3), MCL_TRIM_ALL (7).";
            throw std::runtime_error(ss.str());
        }
    }

    std::string result_str = input_str;

    if (operation & MCL_TRIM_LEFT)
    {
        size_t first_char = result_str.find_first_not_of(characters_to_trim);
        if (first_char == std::string::npos)
        {
            return "";
        }
        result_str = result_str.substr(first_char);
    }

    if (operation & MCL_TRIM_RIGHT)
    {
        size_t last_char = result_str.find_last_not_of(characters_to_trim);
        if (last_char == std::string::npos)
        {
            return "";
        }
        result_str = result_str.substr(0, last_char + 1);
    }

    if (operation & MCL_TRIM_MIDDLE)
    {
        if (!characters_to_trim.empty())
        {
            std::string temp_str;
            temp_str.reserve(result_str.length());
            bool in_trim_sequence = false;
            for (char c : result_str)
            {
                if (characters_to_trim.find(c) != std::string::npos)
                {
                    if (!in_trim_sequence)
                    {
                        temp_str += c;
                        in_trim_sequence = true;
                    }
                }
                else
                {
                    temp_str += c;
                    in_trim_sequence = false;
                }
            }
            result_str = temp_str;
        }
    }

    return result_str;
}

void register_trim_extension(Evaluator &eval)
{
    eval.registerNativeFunction("trim", nirvana_trim);
}