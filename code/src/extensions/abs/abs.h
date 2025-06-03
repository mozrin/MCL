// NEW FILE: src/extensions/abs/abs.h
#ifndef EXTENSION_ABS_H
#define EXTENSION_ABS_H

#include "../../../include/value.h"
#include "../../../include/evaluator.h"

Value nirvana_abs(const std::vector<Value> &args);

void register_abs_extension(Evaluator &eval);

#endif