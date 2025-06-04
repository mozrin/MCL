#ifndef EXTENSION_UPPERCASE_H
#define EXTENSION_UPPERCASE_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_uppercase(const std::vector<Value> &args);

void register_uppercase_extension(Evaluator &eval);

#endif