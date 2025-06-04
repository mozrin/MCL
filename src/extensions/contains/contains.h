#ifndef EXTENSION_CONTAINS_H
#define EXTENSION_CONTAINS_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_contains(const std::vector<Value> &args);

void register_contains_extension(Evaluator &eval);

#endif