#include "../evaluator.h"
#include "../../parser/ast.h"
#include "../../common/value.h"
#include "../../common/token.h"
#include <stdexcept>
#include <variant>
#include <sstream>
#include <cmath>

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