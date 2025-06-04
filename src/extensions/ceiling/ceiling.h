#ifndef EXTENSION_CEILING_H
#define EXTENSION_CEILING_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_ceiling(const std::vector<Value> &args);

void register_ceiling_extension(Evaluator &eval);

#endif