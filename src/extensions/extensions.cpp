#include "extensions.h"

void registerAllExtensions(Evaluator &eval)
{
    register_abs_extension(eval);
    register_echo_extension(eval);
    register_sqrt_extension(eval);
    register_trim_extension(eval);
    register_uppercase_extension(eval);
    register_wrap_extension(eval);
}