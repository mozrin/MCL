#ifndef EXTENSION_MIN_H
#define EXTENSION_MIN_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_min(const std::vector<Value> &args);

void register_min_extension(Evaluator &eval);

#endif