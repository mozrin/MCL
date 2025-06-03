#ifndef ALL_EXTENSIONS_H
#define ALL_EXTENSIONS_H

#include "../core/evaluator.h"

#include "abs/abs.h"
#include "sqrt/sqrt.h"
#include "uppercase/uppercase.h"
#include "trim/trim.h"

void registerAllExtensions(Evaluator &eval);

#endif