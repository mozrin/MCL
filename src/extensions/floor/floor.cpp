#include "floor.h"
#include <cmath>
#include <stdexcept>
#include <variant>

Value mcl_floor(const std::vector<Value> &args)
{
    if (args.size() != 1)
    {
        throw std::runtime_error("Function 'floor' expects exactly 1 argument: floor(number).");
    }

    const Value &val = args[0];

    if (std::holds_alternative<long long>(val))
    {
        return static_cast<long long>(std::floor(static_cast<double>(std::get<long long>(val))));
    }
    else if (std::holds_alternative<double>(val))
    {
        return std::floor(std::get<double>(val));
    }
    else
    {
        throw std::runtime_error("Function 'floor' expects a number (integer or number) argument.");
    }
}

void register_floor_extension(Evaluator &eval)
{
    eval.registerNativeFunction("floor", mcl_floor);
}