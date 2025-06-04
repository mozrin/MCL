#ifndef EXTENSION_ABS_H
#define EXTENSION_ABS_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_abs(const std::vector<Value> &args);

void register_abs_extension(Evaluator &eval);

#endif