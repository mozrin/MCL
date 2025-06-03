#include "extensions.h"

void registerAllExtensions(Evaluator &eval)
{
    register_abs_extension(eval);
    register_sqrt_extension(eval);
}