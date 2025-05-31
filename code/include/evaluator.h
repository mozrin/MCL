// /nirvana/prep_ai/../code/include/evaluator.h
#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include "ast.h"
#include "value.h"

class Evaluator
{
private:
    std::map<std::string, Value> variables;

    Value evaluate(ASTNode *node);
    Value evaluateProgramNode(ProgramNode *node);
    Value evaluateAssignmentStatement(AssignmentStatement *node);
    Value evaluateEchoStatement(EchoStatement *node);
    Value evaluateStringLiteralExpr(StringLiteralExpr *node);
    Value evaluateNumberLiteralExpr(NumberLiteralExpr *node);
    Value evaluateBooleanLiteralExpr(BooleanLiteralExpr *node);
    Value evaluateVariableExpr(VariableExpr *node);
    Value evaluateBinaryOpExpr(BinaryOpExpr *node);
    Value evaluateUnaryOpExpr(UnaryOpExpr *node);

public:
    Evaluator();
    void interpret(std::unique_ptr<ProgramNode> ast);
};

#endif