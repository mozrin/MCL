#ifndef EXTENSION_FLOOR_H
#define EXTENSION_FLOOR_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_floor(const std::vector<Value> &args);

void register_floor_extension(Evaluator &eval);

#endif