#include "pad.h"
#include "../../core/constants.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <algorithm>

Value mcl_pad(const std::vector<Value> &args)
{
    if (args.empty() || args.size() > 4)
    {
        throw std::runtime_error("Function 'pad' expects 2 to 4 arguments: pad(string $str, integer $length, string $pad_string=\" \", integer $pad_type=MCL_PAD_RIGHT).");
    }

    std::string input_string;
    if (std::holds_alternative<std::string>(args[0]))
    {
        input_string = std::get<std::string>(args[0]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'pad': Argument 1 ($str) must be a string, but got value of type ";
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

    long long length;
    if (std::holds_alternative<long long>(args[1]))
    {
        length = std::get<long long>(args[1]);
    }
    else if (std::holds_alternative<double>(args[1]))
    {
        double d_len = std::get<double>(args[1]);
        if (d_len != std::floor(d_len))
        {
            std::stringstream ss;
            ss << "Function 'pad': Argument 2 ($length) must be an integer, but got " << d_len << ".";
            throw std::runtime_error(ss.str());
        }
        length = static_cast<long long>(d_len);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'pad': Argument 2 ($length) must be an integer (integer or number), but got value of type ";
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

    std::string pad_string_val = " ";
    if (args.size() >= 3)
    {
        if (std::holds_alternative<std::string>(args[2]))
        {
            pad_string_val = std::get<std::string>(args[2]);
            if (pad_string_val.empty())
            {
                pad_string_val = " ";
            }
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'pad': Argument 3 ($pad_string) must be a string, but got value of type ";
            switch (valueTypeToDeclaredType(args[2]))
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

    long long pad_type = MCL_PAD_RIGHT;
    if (args.size() == 4)
    {
        if (std::holds_alternative<long long>(args[3]))
        {
            pad_type = std::get<long long>(args[3]);
        }
        else if (std::holds_alternative<double>(args[3]))
        {
            double d_type = std::get<double>(args[3]);
            if (d_type != std::floor(d_type))
            {
                std::stringstream ss;
                ss << "Function 'pad': Argument 4 ($pad_type) must be an integer, but got " << d_type << ".";
                throw std::runtime_error(ss.str());
            }
            pad_type = static_cast<long long>(d_type);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'pad': Argument 4 ($pad_type) must be an integer (integer or number), but got value of type ";
            switch (valueTypeToDeclaredType(args[3]))
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

        if (pad_type != MCL_PAD_LEFT && pad_type != MCL_PAD_RIGHT)
        {
            std::stringstream ss;
            ss << "Function 'pad': Invalid pad type constant " << pad_type << ". Use MCL_PAD_LEFT or MCL_PAD_RIGHT.";
            throw std::runtime_error(ss.str());
        }
    }

    size_t input_len = input_string.length();
    if (length <= static_cast<long long>(input_len))
    {
        return input_string;
    }

    size_t pad_chars_needed = static_cast<size_t>(length - input_len);
    char actual_pad_char = pad_string_val[0];
    std::string padding_str(pad_chars_needed, actual_pad_char);

    if (pad_type == MCL_PAD_RIGHT)
    {
        return input_string + padding_str;
    }
    else if (pad_type == MCL_PAD_LEFT)
    {
        return padding_str + input_string;
    }

    return input_string;
}

void register_pad_extension(Evaluator &eval)
{
    eval.registerNativeFunction("pad", mcl_pad);
}