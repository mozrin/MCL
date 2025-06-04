#ifndef EXTENSION_WRAP_H
#define EXTENSION_WRAP_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_wrap(const std::vector<Value> &args);

void register_wrap_extension(Evaluator &eval);

#endif