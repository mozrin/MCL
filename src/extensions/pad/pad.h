#ifndef EXTENSION_PAD_H
#define EXTENSION_PAD_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_pad(const std::vector<Value> &args);

void register_pad_extension(Evaluator &eval);

#endif