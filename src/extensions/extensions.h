#ifndef ALL_EXTENSIONS_H
#define ALL_EXTENSIONS_H

#include "../core/evaluator.h"

#include "abs/abs.h"
#include "ceiling/ceiling.h"
#include "contains/contains.h"
#include "floor/floor.h"
#include "max/max.h"
#include "min/min.h"
#include "reverse/reverse.h"
#include "sqrt/sqrt.h"
#include "trim/trim.h"
#include "uppercase/uppercase.h"
#include "wrap/wrap.h"

void registerAllExtensions(Evaluator &eval);

#endif