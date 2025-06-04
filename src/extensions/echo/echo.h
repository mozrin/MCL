#ifndef EXTENSION_ECHO_H
#define EXTENSION_ECHO_H

#include "../../core/value.h"
#include "../../core/evaluator.h"

Value mcl_echo(const std::vector<Value> &args);

void register_echo_extension(Evaluator &eval);

#endif