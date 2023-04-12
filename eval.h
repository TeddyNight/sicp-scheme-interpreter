#ifndef __EVAL_H__
#define __EVAL_H__
#include "type.h"
obj eval(obj exp, obj env);
obj list_of_values(obj exps, obj env);
obj eval_if(obj exp, obj env);
obj eval_sequence(obj exps, obj env);
obj eval_assignment(obj exp, obj env);
obj eval_definition(obj exp, obj env);
obj text_of_quotation(obj exp);
int is_number(obj exp);
int is_string(obj exp);
int is_self_evaluating(obj exp);
obj lambda_parameters(obj exp);
obj lambda_body(obj exp);
obj make_lambda(obj parameters, obj body);
obj begin_actions(obj exp);
int is_number(obj exp);
obj cond2if(obj exp);
#endif
