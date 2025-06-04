#ifndef EXTENSION_MAX_H
#define EXTENSION_MAX_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_max(const std::vector<Value> &args);

void register_max_extension(Evaluator &eval);

#endif