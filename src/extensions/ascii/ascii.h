#ifndef EXTENSION_ASCII_H
#define EXTENSION_ASCII_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_ascii(const std::vector<Value> &args);

void register_ascii_extension(Evaluator &eval);

#endif