#include "ceiling.h"
#include <cmath>
#include <stdexcept>
#include <variant>

Value mcl_ceiling(const std::vector<Value> &args)
{
    if (args.size() != 1)
    {
        throw std::runtime_error("Function 'ceiling' expects exactly 1 argument: ceiling(number).");
    }

    const Value &val = args[0];

    if (std::holds_alternative<long long>(val))
    {
        return static_cast<long long>(std::ceil(static_cast<double>(std::get<long long>(val))));
    }
    else if (std::holds_alternative<double>(val))
    {
        return std::ceil(std::get<double>(val));
    }
    else
    {
        throw std::runtime_error("Function 'ceiling' expects a number (integer or number) argument.");
    }
}

void register_ceiling_extension(Evaluator &eval)
{
    eval.registerNativeFunction("ceiling", mcl_ceiling);
}