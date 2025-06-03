#ifndef EXTENSION_TRIM_H
#define EXTENSION_TRIM_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value nirvana_trim(const std::vector<Value> &args);

void register_trim_extension(Evaluator &eval);

#endif