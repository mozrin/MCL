#include "extensions.h"

void registerAllExtensions(Evaluator &eval)
{
    register_abs_extension(eval);
    register_ceiling_extension(eval);
    register_contains_extension(eval);
    register_floor_extension(eval);
    register_max_extension(eval);
    register_min_extension(eval);
    register_pad_extension(eval);
    register_reverse_extension(eval);
    register_sqrt_extension(eval);
    register_trim_extension(eval);
    register_uppercase_extension(eval);
    register_wrap_extension(eval);
}