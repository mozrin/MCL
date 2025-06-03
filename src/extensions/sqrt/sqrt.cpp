#include "sqrt.h"
#include <cmath>
#include <stdexcept>
#include <variant>

Value nirvana_sqrt(const std::vector<Value> &args)
{
    if (args.size() != 1)
    {
        throw std::runtime_error("Function 'sqrt' expects exactly 1 argument: sqrt(number).");
    }

    const Value &val = args[0];

    if (std::holds_alternative<long long>(val))
    {
        long long ll_val = std::get<long long>(val);
        if (ll_val < 0)
            throw std::runtime_error("Function 'sqrt': Argument cannot be negative.");
        return std::sqrt(static_cast<double>(ll_val));
    }
    else if (std::holds_alternative<double>(val))
    {
        double d_val = std::get<double>(val);
        if (d_val < 0.0)
            throw std::runtime_error("Function 'sqrt': Argument cannot be negative.");
        return std::sqrt(d_val);
    }
    else
    {
        throw std::runtime_error("Function 'sqrt' expects a number (integer or number) argument.");
    }
}

void register_sqrt_extension(Evaluator &eval)
{
    eval.registerNativeFunction("sqrt", nirvana_sqrt);
}