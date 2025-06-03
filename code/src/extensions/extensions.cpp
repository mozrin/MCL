// CHANGED FILE: src/extensions/extensions.cpp
#include "extensions.h"
#include "abs/abs.h"

void registerAllExtensions(Evaluator &eval)
{
    register_abs_extension(eval);
}