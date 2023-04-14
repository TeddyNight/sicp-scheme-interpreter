#ifndef __APPLY_H__
#define __APPLY_H__
#include "eval.h"
#include "type.h"
obj apply(obj proc, obj args);

int is_primitive_procedure(obj proc);
int is_compound_procedure(obj proc);

obj apply_primitive_procedure(obj proc, obj args);
obj eval_sequence(obj body, obj proc);
obj procedure_body(obj proc);
obj procedure_parameters(obj proc);
obj procedure_environment(obj proc);

obj primitive_procedure_implementation(obj proc);

obj make_procedure(obj proc, obj body, obj env);
#endif
