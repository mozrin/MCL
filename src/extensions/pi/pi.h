#ifndef EXTENSION_PI_H
#define EXTENSION_PI_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_pi(const std::vector<Value> &args);

void register_pi_extension(Evaluator &eval);

#endif