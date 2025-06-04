#ifndef EXTENSION_CHARACTER_H
#define EXTENSION_CHARACTER_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_character(const std::vector<Value> &args);

void register_character_extension(Evaluator &eval);

#endif