#ifndef EXTENSION_MIN_H
#define EXTENSION_MIN_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_min(const std::vector<Value> &args);

void register_min_extension(Evaluator &eval);

#endif