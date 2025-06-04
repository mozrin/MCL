#include "wrap.h"
#include "../../core/common/constants.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>

Value mcl_wrap(const std::vector<Value> &args)
{
    if (args.empty() || args.size() > 2)
    {
        throw std::runtime_error("Function 'wrap' expects 1 or 2 arguments: wrap(string $value, string $characters=\"\").");
    }

    std::string input_string;
    if (std::holds_alternative<std::string>(args[0]))
    {
        input_string = std::get<std::string>(args[0]);
    }
    else
    {
        std::stringstream ss;
        ss << "Function 'wrap': Argument 1 ($value) must be a string, but got value of type ";
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

    std::string wrapper_chars = "";
    if (args.size() == 2)
    {
        if (std::holds_alternative<std::string>(args[1]))
        {
            wrapper_chars = std::get<std::string>(args[1]);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'wrap': Argument 2 ($characters) must be a string, but got value of type ";
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

    std::string left_wrapper = "";
    std::string right_wrapper = "";

    size_t pipe_pos = wrapper_chars.find('|');

    if (pipe_pos == std::string::npos)
    {
        left_wrapper = wrapper_chars;
    }
    else
    {
        left_wrapper = wrapper_chars.substr(0, pipe_pos);
        right_wrapper = wrapper_chars.substr(pipe_pos + 1);
    }

    std::string result = left_wrapper + input_string + right_wrapper;

    return result;
}

void register_wrap_extension(Evaluator &eval)
{
    eval.registerNativeFunction("wrap", mcl_wrap);
}