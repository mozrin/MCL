#include "pi.h"
#include "pi_constants.h"
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <iomanip>

const double MCL_PI_FULL_VALUE = 3.14159265358979;

Value mcl_pi(const std::vector<Value> &args)
{
    long long significant_places = MCL_PI_PRECISION_DEFAULT;

    if (args.size() > 1)
    {
        throw std::runtime_error("Function 'pi' expects 0 or 1 argument: pi(integer $significant_places=" + std::to_string(MCL_PI_PRECISION_DEFAULT) + ").");
    }

    if (args.size() == 1)
    {
        if (std::holds_alternative<long long>(args[0]))
        {
            significant_places = std::get<long long>(args[0]);
        }
        else if (std::holds_alternative<double>(args[0]))
        {
            double d_places = std::get<double>(args[0]);
            if (d_places != std::floor(d_places))
            {
                std::stringstream ss;
                ss << "Function 'pi': Argument 1 ($significant_places) must be an integer, but got " << d_places << ".";
                throw std::runtime_error(ss.str());
            }
            significant_places = static_cast<long long>(d_places);
        }
        else
        {
            std::stringstream ss;
            ss << "Function 'pi': Argument 1 ($significant_places) must be an integer (integer or number), but got value of type ";
            switch (valueTypeToDeclaredType(args[0]))
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
    }

    if (significant_places < 0 || significant_places > MCL_PI_PRECISION_DEFAULT)
    {
        std::stringstream ss;
        ss << "Function 'pi': Argument 1 ($significant_places) must be an integer between 0 and " << MCL_PI_PRECISION_DEFAULT << " (inclusive), but got " << significant_places << ".";
        throw std::runtime_error(ss.str());
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(MCL_PI_PRECISION_DEFAULT) << MCL_PI_FULL_VALUE;
    std::string pi_str = ss.str();

    size_t target_len;
    if (significant_places == 0)
    {
        target_len = pi_str.find('.');
        if (target_len == std::string::npos)
            target_len = pi_str.length();
    }
    else
    {
        target_len = 1 + 1 + static_cast<size_t>(significant_places);
    }

    std::string clipped_pi_str = pi_str.substr(0, target_len);

    return std::stod(clipped_pi_str);
}

void register_pi_extension(Evaluator &eval)
{
    eval.registerNativeFunction("pi", mcl_pi);
}