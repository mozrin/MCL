// NEW FILE: src/extensions/abs/abs.cpp
#include "abs.h"
#include <cmath>
#include <stdexcept>
#include <variant>

Value nirvana_abs(const std::vector<Value> &args)
{
    if (args.size() != 1)
    {
        throw std::runtime_error("Function 'abs' expects exactly 1 argument: abs(number).");
    }

    const Value &val = args[0];

    if (std::holds_alternative<long long>(val))
    {
        return std::abs(std::get<long long>(val));
    }
    else if (std::holds_alternative<double>(val))
    {
        return std::abs(std::get<double>(val));
    }
    else
    {
        throw std::runtime_error("Function 'abs' expects a number (integer or number) argument.");
    }
}

void register_abs_extension(Evaluator &eval)
{
    eval.registerNativeFunction("abs", nirvana_abs);
}