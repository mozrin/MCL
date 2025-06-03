#ifndef EXTENSION_SQRT_H
#define EXTENSION_SQRT_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value nirvana_sqrt(const std::vector<Value> &args);

void register_sqrt_extension(Evaluator &eval);

#endif