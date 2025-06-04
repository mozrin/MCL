#ifndef EXTENSION_CEILING_H
#define EXTENSION_CEILING_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_ceiling(const std::vector<Value> &args);

void register_ceiling_extension(Evaluator &eval);

#endif