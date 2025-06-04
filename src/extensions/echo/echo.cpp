#include "echo.h"
#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <stdexcept>
#include <sstream>

Value mcl_echo(const std::vector<Value> &args)
{
    if (args.empty())
    {
        throw std::runtime_error("Function 'echo' expects at least 1 argument.");
    }

    for (size_t i = 0; i < args.size(); ++i)
    {
        std::cout << args[i];
        if (i < args.size() - 1)
        {
            std::cout << " ";
        }
    }
    std::cout << std::endl;

    return std::monostate{};
}

void register_echo_extension(Evaluator &eval)
{
    eval.registerNativeFunction("echo", mcl_echo);
}