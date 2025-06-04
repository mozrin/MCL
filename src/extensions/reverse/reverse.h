#ifndef EXTENSION_REVERSE_H
#define EXTENSION_REVERSE_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_reverse(const std::vector<Value> &args);

void register_reverse_extension(Evaluator &eval);

#endif