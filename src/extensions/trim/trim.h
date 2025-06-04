#ifndef EXTENSION_TRIM_H
#define EXTENSION_TRIM_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_trim(const std::vector<Value> &args);

void register_trim_extension(Evaluator &eval);

#endif