#ifndef EXTENSION_PI_H
#define EXTENSION_PI_H

#include "../../core/common/value.h"
#include "../../core/runtime/evaluator.h"

Value mcl_pi(const std::vector<Value> &args);

void register_pi_extension(Evaluator &eval);

#endif