#include "evaluator.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <variant>
#include <limits>

bool Evaluator::convertToBool(const Value &val)
{
    return std::visit([](auto &&arg) -> bool
    {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::string>) {
            return !arg.empty();
        } else if constexpr (std::is_same_v<T, long long>) {
            return arg != 0LL;
        } else if constexpr (std::is_same_v<T, double>) {
            return arg != 0.0;
        } else if constexpr (std::is_same_v<T, bool>) {
            return arg;
        } else if constexpr (std::is_same_v<T, std::monostate>) {
            return false;
        } else {
            throw std::runtime_error("Internal error: convertToBool received unsupported type.");
        } }, val);
}

Value Evaluator::performNumericBinaryOp(const Value &left_val, const Value &right_val, TokenType op)
{
    long long left_ll = 0;
    bool left_is_ll_candidate = false;
    if (std::holds_alternative<long long>(left_val))
    {
        left_ll = std::get<long long>(left_val);
        left_is_ll_candidate = true;
    }
    else if (std::holds_alternative<bool>(left_val))
    {
        left_ll = std::get<bool>(left_val) ? 1LL : 0LL;
        left_is_ll_candidate = true;
    }
    else if (std::holds_alternative<double>(left_val))
    {
        left_is_ll_candidate = false;
    }

    long long right_ll = 0;
    bool right_is_ll_candidate = false;
    if (std::holds_alternative<long long>(right_val))
    {
        right_ll = std::get<long long>(right_val);
        right_is_ll_candidate = true;
    }
    else if (std::holds_alternative<bool>(right_val))
    {
        right_ll = std::get<bool>(right_val) ? 1LL : 0LL;
        right_is_ll_candidate = true;
    }
    else if (std::holds_alternative<double>(right_val))
    {
        right_is_ll_candidate = false;
    }

    if (left_is_ll_candidate && right_is_ll_candidate)
    {
        switch (op)
        {
        case TokenType::PLUS:
            return left_ll + right_ll;
        case TokenType::MINUS:
            return left_ll - right_ll;
        case TokenType::STAR:
            return left_ll * right_ll;
        case TokenType::SLASH:
            if (right_ll == 0LL)
                throw std::runtime_error("Runtime error: Integer division by zero.");
            return left_ll / right_ll;
        default:
            break;
        }
    }

    double left_double_val;
    if (std::holds_alternative<double>(left_val))
        left_double_val = std::get<double>(left_val);
    else
        left_double_val = static_cast<double>(left_ll);

    double right_double_val;
    if (std::holds_alternative<double>(right_val))
        right_double_val = std::get<double>(right_val);
    else
        right_double_val = static_cast<double>(right_ll);

    switch (op)
    {
    case TokenType::PLUS:
        return left_double_val + right_double_val;
    case TokenType::MINUS:
        return left_double_val - right_double_val;
    case TokenType::STAR:
        return left_double_val * right_double_val;
    case TokenType::SLASH:
        if (right_double_val == 0.0)
            throw std::runtime_error("Runtime error: Floating-point division by zero.");
        return left_double_val / right_double_val;
    case TokenType::GREATER:
        return left_double_val > right_double_val;
    case TokenType::GREATER_EQUAL:
        return left_double_val >= right_double_val;
    case TokenType::LESS:
        return left_double_val < right_double_val;
    case TokenType::LESS_EQUAL:
        return left_double_val <= right_double_val;
    default:
        throw std::runtime_error("Unsupported numeric binary operator.");
    }
}

Value Evaluator::performBooleanBinaryOp(const Value &left_val, const Value &right_val, TokenType op)
{
    bool left_bool = convertToBool(left_val);
    bool right_bool = convertToBool(right_val);

    switch (op)
    {
    case TokenType::AND:
        return left_bool && right_bool;
    case TokenType::OR:
        return left_bool || right_bool;
    default:
        throw std::runtime_error("Unsupported boolean binary operator.");
    }
}

void Evaluator::enforceType(const std::string &var_name, DeclaredType declared_type, const Value &assigned_value)
{
    DeclaredType actual_type = valueTypeToDeclaredType(assigned_value);

    if (declared_type == DeclaredType::ANY)
    {
        return;
    }

    bool type_matches = false;
    switch (declared_type)
    {
    case DeclaredType::STRING:
        type_matches = (actual_type == DeclaredType::STRING);
        break;
    case DeclaredType::INTEGER:
        type_matches = (actual_type == DeclaredType::INTEGER || actual_type == DeclaredType::BOOLEAN);
        if (actual_type == DeclaredType::NUMBER)
        {
            double d_val = std::get<double>(assigned_value);
            type_matches = (d_val == std::floor(d_val) && d_val >= std::numeric_limits<long long>::min() && d_val <= std::numeric_limits<long long>::max());
        }
        break;
    case DeclaredType::NUMBER:
        type_matches = (actual_type == DeclaredType::NUMBER || actual_type == DeclaredType::INTEGER || actual_type == DeclaredType::BOOLEAN);
        break;
    case DeclaredType::BOOLEAN:
        type_matches = (actual_type == DeclaredType::BOOLEAN);
        if (actual_type == DeclaredType::INTEGER)
        {
            long long ll_val = std::get<long long>(assigned_value);
            type_matches = (ll_val == 0LL || ll_val == 1LL);
        }
        break;
    default:
        throw std::runtime_error("Internal error: Unknown declared type for variable '" + var_name + "'.");
    }

    if (!type_matches)
    {
        std::stringstream ss;
        ss << "Type mismatch for variable '" << var_name << "'. Expected ";
        switch (declared_type)
        {
        case DeclaredType::STRING:
            ss << "string";
            break;
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
            ss << "unknown declared type";
            break;
        }
        ss << ", but got value of type ";
        switch (actual_type)
        {
        case DeclaredType::STRING:
            ss << "string";
            break;
        case DeclaredType::INTEGER:
            ss << "integer";
            break;
        case DeclaredType::NUMBER:
            ss << "number";
            break;
        case DeclaredType::BOOLEAN:
            ss << "boolean";
            break;
        case DeclaredType::ANY:
            ss << "null";
            break;
        default:
            ss << "unknown actual type";
            break;
        }
        throw std::runtime_error(ss.str());
    }
}

Value Evaluator::getDefaultValueForType(TokenType type_token)
{
    switch (type_token)
    {
    case TokenType::STRING_KEYWORD:
        return std::string("");
    case TokenType::INTEGER_KEYWORD:
        return 0LL;
    case TokenType::NUMBER_KEYWORD:
        return 0.0;
    case TokenType::BOOLEAN_KEYWORD:
        return false;
    default:
        return std::monostate{};
    }
}

Evaluator::Evaluator()
{
    enterScope();
}

void Evaluator::enterScope()
{
    scopeStack.emplace_back();
}

void Evaluator::exitScope()
{
    if (scopeStack.empty())
    {
        throw std::runtime_error("Internal error: Attempted to exit empty scope stack.");
    }
    scopeStack.pop_back();
}

std::pair<Value *, DeclaredType *> Evaluator::findVariableInScope(const std::string &name)
{
    for (auto it = scopeStack.rbegin(); it != scopeStack.rend(); ++it)
    {
        auto var_it = it->find(name);
        if (var_it != it->end())
        {
            return {&var_it->second.first, &var_it->second.second};
        }
    }
    return {nullptr, nullptr};
}

void Evaluator::registerNativeFunction(const std::string &name, NativeFunction func)
{
    nativeFunctions[name] = std::move(func);
}

void Evaluator::registerConstant(const std::string &name, Value value)
{
    if (findVariableInScope(name).first != nullptr)
    {
        throw std::runtime_error("Internal error: Attempted to re-register existing constant/variable '" + name + "'.");
    }

    scopeStack.front()[name] = {value, valueTypeToDeclaredType(value)};
}

Value Evaluator::evaluate(ASTNode *node)
{
    if (!node)
    {
        return std::monostate{};
    }

    if (auto *prog = dynamic_cast<ProgramNode *>(node))
    {
        return evaluateProgramNode(prog);
    }
    else if (auto *funcDecl = dynamic_cast<FunctionDeclaration *>(node))
    {
        return evaluateFunctionDeclaration(funcDecl);
    }
    else if (auto *block = dynamic_cast<BlockStatement *>(node))
    {
        return evaluateBlockStatement(block);
    }
    else if (auto *ret = dynamic_cast<ReturnStatement *>(node))
    {
        return evaluateReturnStatement(ret);
    }
    else if (auto *decl = dynamic_cast<DeclarationStatement *>(node))
    {
        return evaluateDeclarationStatement(decl);
    }
    else if (auto *assign = dynamic_cast<AssignmentStatement *>(node))
    {
        return evaluateAssignmentStatement(assign);
    }
    else if (auto *echo = dynamic_cast<EchoStatement *>(node))
    {
        return evaluateEchoStatement(echo);
    }
    else if (auto *strLit = dynamic_cast<StringLiteralExpr *>(node))
    {
        return evaluateStringLiteralExpr(strLit);
    }
    else if (auto *numLit = dynamic_cast<NumberLiteralExpr *>(node))
    {
        return evaluateNumberLiteralExpr(numLit);
    }
    else if (auto *boolLit = dynamic_cast<BooleanLiteralExpr *>(node))
    {
        return evaluateBooleanLiteralExpr(boolLit);
    }
    else if (auto *var = dynamic_cast<VariableExpr *>(node))
    {
        return evaluateVariableExpr(var);
    }
    else if (auto *binOp = dynamic_cast<BinaryOpExpr *>(node))
    {
        return evaluateBinaryOpExpr(binOp);
    }
    else if (auto *unaryOp = dynamic_cast<UnaryOpExpr *>(node))
    {
        return evaluateUnaryOpExpr(unaryOp);
    }
    else if (auto *call = dynamic_cast<CallExpr *>(node))
    {
        return evaluateCallExpr(call);
    }
    else
    {
        throw std::runtime_error("Unknown AST node type during evaluation.");
    }
}

Value Evaluator::evaluateProgramNode(ProgramNode *node)
{
    for (const auto &stmt : node->statements)
    {
        evaluate(stmt.get());
    }
    return std::monostate{};
}

Value Evaluator::evaluateFunctionDeclaration(FunctionDeclaration *node)
{
    if (userFunctions.count(node->name) || nativeFunctions.count(node->name))
    {
        throw std::runtime_error("Runtime error: Function or native function '" + node->name + "' already declared.");
    }
    userFunctions[node->name] = node;
    return std::monostate{};
}

Value Evaluator::evaluateBlockStatement(BlockStatement *node)
{
    for (const auto &stmt : node->statements)
    {
        try
        {
            evaluate(stmt.get());
        }
        catch (const FunctionReturnException &)
        {
            throw;
        }
    }
    return std::monostate{};
}

Value Evaluator::evaluateReturnStatement(ReturnStatement *node)
{
    Value ret_val = std::monostate{};
    if (node->expression)
    {
        ret_val = evaluate(node->expression.get());
    }
    throw FunctionReturnException(std::move(ret_val));
}

Value Evaluator::evaluateDeclarationStatement(DeclarationStatement *node)
{
    std::string var_name = node->target->name;
    DeclaredType declared_type = tokenTypeToDeclaredType(node->declared_type);

    if (scopeStack.back().count(var_name))
    {
        throw std::runtime_error("Runtime error: Variable '" + var_name + "' already declared in this scope.");
    }

    Value initial_value_resolved = std::monostate{};
    if (node->value)
    {
        initial_value_resolved = evaluate(node->value.get());
    }
    else
    {
        initial_value_resolved = getDefaultValueForType(node->declared_type);
    }

    enforceType(var_name, declared_type, initial_value_resolved);
    scopeStack.back()[var_name] = {initial_value_resolved, declared_type};
    return initial_value_resolved;
}

Value Evaluator::evaluateAssignmentStatement(AssignmentStatement *node)
{
    Value value = evaluate(node->value.get());
    std::string var_name = node->target->name;

    auto [val_ptr, type_ptr] = findVariableInScope(var_name);

    if (val_ptr == nullptr)
    {
        scopeStack.back()[var_name] = {value, DeclaredType::ANY};
    }
    else
    {
        enforceType(var_name, *type_ptr, value);
        *val_ptr = value;
    }
    return value;
}

Value Evaluator::evaluateEchoStatement(EchoStatement *node)
{
    Value result = evaluate(node->expression.get());
    std::cout << result << std::endl;
    return std::monostate{};
}

Value Evaluator::evaluateStringLiteralExpr(StringLiteralExpr *node)
{
    return node->value;
}

Value Evaluator::evaluateNumberLiteralExpr(NumberLiteralExpr *node)
{
    return node->value;
}

Value Evaluator::evaluateBooleanLiteralExpr(BooleanLiteralExpr *node)
{
    return node->value;
}

Value Evaluator::evaluateVariableExpr(VariableExpr *node)
{
    auto [val_ptr, type_ptr] = findVariableInScope(node->name);
    if (val_ptr != nullptr)
    {
        return *val_ptr;
    }
    else
    {
        throw std::runtime_error("Undefined variable: " + node->name);
    }
}

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
                for (const auto& param_decl : func_decl->parameters)
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

Value Evaluator::evaluateUnaryOpExpr(UnaryOpExpr *node)
{
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::MINUS)
    {
        return std::visit([](auto &&arg) -> Value
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, long long>) {
                return -arg;
            } else if constexpr (std::is_same_v<T, double>) {
                return -arg;
            } else if constexpr (std::is_same_v<T, bool>) {
                return static_cast<long long>(arg ? -1 : 0);
            } else {
                throw std::runtime_error("Type error: Unary '-' operator can only be applied to numbers.");
            } }, right_val);
    }
    else if (node->op == TokenType::BANG || node->op == TokenType::NOT)
    {
        return !convertToBool(right_val);
    }
    throw std::runtime_error("Unsupported unary operator.");
}

Value Evaluator::evaluateBinaryOpExpr(BinaryOpExpr *node)
{
    Value left_val = evaluate(node->left.get());
    Value right_val = evaluate(node->right.get());

    if (node->op == TokenType::DOT)
    {
        std::string s_left, s_right;
        std::visit([&](auto &&arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                s_left = arg;
            } else {
                std::stringstream ss;
                ss << arg;
                s_left = ss.str();
            } }, left_val);
        std::visit([&](auto &&arg)
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::string>) {
                s_right = arg;
            } else {
                std::stringstream ss;
                ss << arg;
                s_right = ss.str();
            } }, right_val);
        return s_left + s_right;
    }
    else if (node->op == TokenType::PLUS || node->op == TokenType::MINUS ||
             node->op == TokenType::STAR || node->op == TokenType::SLASH ||
             node->op == TokenType::GREATER || node->op == TokenType::GREATER_EQUAL ||
             node->op == TokenType::LESS || node->op == TokenType::LESS_EQUAL)
    {

        bool left_is_numeric_like = std::holds_alternative<long long>(left_val) || std::holds_alternative<double>(left_val) || std::holds_alternative<bool>(left_val);
        bool right_is_numeric_like = std::holds_alternative<long long>(right_val) || std::holds_alternative<double>(right_val) || std::holds_alternative<bool>(right_val);

        if (!left_is_numeric_like || !right_is_numeric_like)
        {
            throw std::runtime_error("Type error: Arithmetic/comparison operators can only be applied to numbers.");
        }

        return performNumericBinaryOp(left_val, right_val, node->op);
    }
    else if (node->op == TokenType::EQUAL_EQUAL || node->op == TokenType::BANG_EQUAL)
    {

        bool are_equal = std::visit([](auto &&l_arg, auto &&r_arg) -> bool
        {

            using L = std::decay_t<decltype(l_arg)>;
            using R = std::decay_t<decltype(r_arg)>;

            if constexpr (std::is_same_v<L, R>) {
                if constexpr (std::is_same_v<L, std::string>) return l_arg == r_arg;
                else if constexpr (std::is_same_v<L, long long>) return l_arg == r_arg;
                else if constexpr (std::is_same_v<L, double>) return std::abs(l_arg - r_arg) < 0.000001;
                else if constexpr (std::is_same_v<L, bool>) return l_arg == r_arg;
                else if constexpr (std::is_same_v<L, std::monostate>) return true;
            }

            else if constexpr (
                (std::is_same_v<L, long long> || std::is_same_v<L, double> || std::is_same_v<L, bool>) &&
                (std::is_same_v<R, long long> || std::is_same_v<R, double> || std::is_same_v<R, bool>)
            ) {
                double l_double, r_double;
                if constexpr (std::is_same_v<L, long long>) l_double = static_cast<double>(l_arg);
                else if constexpr (std::is_same_v<L, double>) l_double = l_arg;
                else if constexpr (std::is_same_v<L, bool>) l_double = l_arg ? 1.0 : 0.0;
                else l_double = 0.0;

                if constexpr (std::is_same_v<R, long long>) r_double = static_cast<double>(r_arg);
                else if constexpr (std::is_same_v<R, double>) r_double = r_arg;
                else if constexpr (std::is_same_v<R, bool>) r_double = r_arg ? 1.0 : 0.0;
                else r_double = 0.0;

                return std::abs(l_double - r_double) < 0.000001;
            }

            return false; }, left_val, right_val);

        return (node->op == TokenType::EQUAL_EQUAL) ? are_equal : !are_equal;
    }
    else if (node->op == TokenType::AND || node->op == TokenType::OR)
    {

        return performBooleanBinaryOp(left_val, right_val, node->op);
    }
    else if (node->op == TokenType::PIPE)
    {
        long long left_int_val;
        long long right_int_val;

        if (std::holds_alternative<long long>(left_val))
        {
            left_int_val = std::get<long long>(left_val);
        }
        else if (std::holds_alternative<double>(left_val))
        {
            double d_val = std::get<double>(left_val);
            long long ll_val = static_cast<long long>(d_val);
            if (d_val != static_cast<double>(ll_val))
            {
                throw std::runtime_error("Type error: Bitwise OR operator '|' requires integer operands. Left operand is a non-integer number.");
            }
            left_int_val = ll_val;
        }
        else if (std::holds_alternative<bool>(left_val))
        {
            left_int_val = std::get<bool>(left_val) ? 1LL : 0LL;
        }
        else
        {
            throw std::runtime_error("Type error: Bitwise OR operator '|' can only be applied to integer or number (convertible to integer) operands. Left operand is of invalid type.");
        }

        if (std::holds_alternative<long long>(right_val))
        {
            right_int_val = std::get<long long>(right_val);
        }
        else if (std::holds_alternative<double>(right_val))
        {
            double d_val = std::get<double>(right_val);
            long long ll_val = static_cast<long long>(d_val);
            if (d_val != static_cast<double>(ll_val))
            {
                throw std::runtime_error("Type error: Bitwise OR operator '|' requires integer operands. Right operand is a non-integer number.");
            }
            right_int_val = ll_val;
        }
        else if (std::holds_alternative<bool>(right_val))
        {
            right_int_val = std::get<bool>(right_val) ? 1LL : 0LL;
        }
        else
        {
            throw std::runtime_error("Type error: Bitwise OR operator '|' can only be applied to integer or number (convertible to integer) operands. Right operand is of invalid type.");
        }

        return left_int_val | right_int_val;
    }

    throw std::runtime_error("Unsupported binary operator.");
}

void Evaluator::interpret(std::unique_ptr<ProgramNode> ast)
{
    evaluate(ast.get());
}

Value Evaluator::callNativeFunctionByName(const std::string &name, const std::vector<Value> &args)
{
    auto it = nativeFunctions.find(name);
    if (it == nativeFunctions.end())
    {
        throw std::runtime_error("Runtime error: Attempted to call unknown native function '" + name + "'.");
    }
    return it->second(args);
}

Value Evaluator::getConstant(const std::string &name)
{
    auto [val_ptr, type_ptr] = findVariableInScope(name);
    if (val_ptr == nullptr)
    {
        throw std::runtime_error("Runtime error: Undefined constant '" + name + "'.");
    }
    return *val_ptr;
}