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
    Value evaluateVariableExpr(VariableExpr *node);
    Value evaluateBinaryOpExpr(BinaryOpExpr *node);

public:
    Evaluator();
    void interpret(std::unique_ptr<ProgramNode> ast);
};

#endif