#ifndef EXTENSION_REVERSE_H
#define EXTENSION_REVERSE_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_reverse(const std::vector<Value> &args);

void register_reverse_extension(Evaluator &eval);

#endif