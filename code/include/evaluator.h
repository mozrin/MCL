// /nirvana/prep_ai/../code/include/evaluator.h
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "ast.h"
#include "value.h"
#include <functional>
#include <stdexcept>

class Evaluator
{
private:
std::map<std::string, std::pair<Value, DeclaredType>> variables;

Value evaluate(ASTNode *node);
Value evaluateProgramNode(ProgramNode *node);
Value evaluateDeclarationStatement(DeclarationStatement *node);
Value evaluateAssignmentStatement(AssignmentStatement *node);
Value evaluateEchoStatement(EchoStatement *node);
Value evaluateStringLiteralExpr(StringLiteralExpr *node);
Value evaluateNumberLiteralExpr(NumberLiteralExpr *node);
Value evaluateBooleanLiteralExpr(BooleanLiteralExpr *node);
Value evaluateVariableExpr(VariableExpr *node);
Value evaluateBinaryOpExpr(BinaryOpExpr *node);
Value evaluateUnaryOpExpr(UnaryOpExpr *node);

// Helper for type promotion and operation dispatch
Value performNumericBinaryOp(const Value& left, const Value& right, TokenType op);
Value performBooleanBinaryOp(const Value& left, const Value& right, TokenType op);
bool convertToBool(const Value& val);
void enforceType(const std::string& var_name, DeclaredType declared_type, const Value& assigned_value);


public:
Evaluator();
void interpret(std::unique_ptr<ProgramNode> ast);
};

#endif