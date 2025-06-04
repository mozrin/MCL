#ifndef EXTENSION_CHARACTER_H
#define EXTENSION_CHARACTER_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_character(const std::vector<Value> &args);

void register_character_extension(Evaluator &eval);

#endif