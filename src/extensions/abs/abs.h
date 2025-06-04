#ifndef EXTENSION_ABS_H
#define EXTENSION_ABS_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_abs(const std::vector<Value> &args);

void register_abs_extension(Evaluator &eval);

#endif