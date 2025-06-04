#include "../evaluator.h"
#include "../../parser/ast.h"
#include "../../common/value.h"
#include "../../common/token.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <variant>
#include <sstream>

Value Evaluator::evaluateCallExpr(CallExpr *node)
{
    if (auto *callee_var = dynamic_cast<VariableExpr *>(node->callee.get()))
    {
        std::string function_name = callee_var->name;
        auto native_it = nativeFunctions.find(function_name);
        auto user_it = userFunctions.find(function_name);

        if (native_it != nativeFunctions.end())
        {
            std::vector<Value> args;
            for (const auto &arg_node : node->arguments)
            {
                args.push_back(evaluate(arg_node.get()));
            }
            return native_it->second(args);
        }
        else if (user_it != userFunctions.end())
        {
            FunctionDeclaration *func_decl = user_it->second;
            enterScope();
            Value result_val = std::monostate{};

            try
            {
                size_t user_arg_idx = 0;
                for (const auto &param_decl : func_decl->parameters)
                {
                    Value param_val;
                    if (user_arg_idx < node->arguments.size())
                    {
                        param_val = evaluate(node->arguments[user_arg_idx].get());
                    }
                    else if (param_decl.default_value)
                    {
                        param_val = evaluate(param_decl.default_value.get());
                    }
                    else
                    {
                        throw std::runtime_error("Runtime error: Missing required argument for parameter '" + param_decl.name + "' in function '" + func_decl->name + "'.");
                    }

                    enforceType(param_decl.name, tokenTypeToDeclaredType(param_decl.declared_type), param_val);
                    scopeStack.back()[param_decl.name] = {param_val, tokenTypeToDeclaredType(param_decl.declared_type)};
                    user_arg_idx++;
                }

                if (user_arg_idx < node->arguments.size())
                {
                    throw std::runtime_error("Runtime error: Too many arguments provided for function '" + func_decl->name + "'. Expected " + std::to_string(func_decl->parameters.size()) + ", but got " + std::to_string(node->arguments.size()) + ".");
                }

                evaluate(func_decl->body.get());

                result_val = getDefaultValueForType(func_decl->return_type);
            }
            catch (const FunctionReturnException &e)
            {
                result_val = e.returnValue;
            }

            enforceType("function '" + func_decl->name + "' return value", tokenTypeToDeclaredType(func_decl->return_type), result_val);
            exitScope();
            return result_val;
        }
        else
        {
            throw std::runtime_error("Runtime error: Undefined function '" + function_name + "'.");
        }
    }
    else
    {
        throw std::runtime_error("Runtime error: Callee in function call is not a simple identifier.");
    }
}