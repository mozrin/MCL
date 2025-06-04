#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include "ast.h"
#include "value.h"
#include <functional>
#include <stdexcept>

using NativeFunction = std::function<Value(const std::vector<Value> &)>;

class FunctionReturnException : public std::runtime_error
{
public:
    Value returnValue;
    FunctionReturnException(Value val) : std::runtime_error("Function return"), returnValue(std::move(val)) {}
};

class Evaluator
{
private:
    std::vector<std::map<std::string, std::pair<Value, DeclaredType>>> scopeStack;
    std::map<std::string, NativeFunction> nativeFunctions;
    std::map<std::string, FunctionDeclaration *> userFunctions;

    Value evaluate(ASTNode *node);
    Value evaluateProgramNode(ProgramNode *node);
    Value evaluateDeclarationStatement(DeclarationStatement *node);
    Value evaluateAssignmentStatement(AssignmentStatement *node);
    Value evaluateEchoStatement(EchoStatement *node);
    Value evaluateReturnStatement(ReturnStatement *node);
    Value evaluateBlockStatement(BlockStatement *node);
    Value evaluateFunctionDeclaration(FunctionDeclaration *node);

    Value evaluateStringLiteralExpr(StringLiteralExpr *node);
    Value evaluateNumberLiteralExpr(NumberLiteralExpr *node);
    Value evaluateBooleanLiteralExpr(BooleanLiteralExpr *node);
    Value evaluateVariableExpr(VariableExpr *node);
    Value evaluateBinaryOpExpr(BinaryOpExpr *node);
    Value evaluateUnaryOpExpr(UnaryOpExpr *node);
    Value evaluateCallExpr(CallExpr *node);

    Value performNumericBinaryOp(const Value &left, const Value &right, TokenType op);
    Value performBooleanBinaryOp(const Value &left, const Value &right, TokenType op);
    bool convertToBool(const Value &val);
    void enforceType(const std::string &var_name, DeclaredType declared_type, const Value &assigned_value);
    Value getDefaultValueForType(TokenType type_token);

    void enterScope();
    void exitScope();
    std::pair<Value*, DeclaredType*> findVariableInScope(const std::string& name);


public:
    Evaluator();
    void registerNativeFunction(const std::string &name, NativeFunction func);
    void registerConstant(const std::string &name, Value value);
    void interpret(std::unique_ptr<ProgramNode> ast);
    Value callNativeFunctionByName(const std::string &name, const std::vector<Value> &args);
    Value getConstant(const std::string &name);
};

#endif