// /nirvana/prep_ai/../code/src/extensions/uppercase/uppercase.h
#ifndef EXTENSION_UPPERCASE_H
#define EXTENSION_UPPERCASE_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value nirvana_uppercase(const std::vector<Value> &args);

void register_uppercase_extension(Evaluator &eval);

#endif