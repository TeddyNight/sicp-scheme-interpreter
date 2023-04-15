#ifndef __EVAL_H__
#define __EVAL_H__
#include "type.h"
obj eval(obj exp, obj env);
obj list_of_values(obj exps, obj env);
obj eval_if(obj exp, obj env);
obj eval_sequence(obj exps, obj env);
obj eval_assignment(obj exp, obj env);
obj eval_definition(obj exp, obj env);
obj definition_variable(obj exp);
obj definition_value(obj exp);
obj text_of_quotation(obj exp);
int is_number(obj exp);
int is_string(obj exp);
int is_self_evaluating(obj exp);
int is_tagged_list(obj proc,char *tag);
int is_variable(obj exp);
int is_quoted(obj exp);
int is_assignment(obj exp);
int is_definition(obj exp);
int is_if(obj exp);
int is_lambda(obj exp);
int is_begin(obj exp);
int is_cond(obj exp);
int is_application(obj exp);
int is_last_exp(obj exp);
int is_symbol(obj exp);
int is_cond_else_clause(obj exp);
obj lambda_parameters(obj exp);
obj lambda_body(obj exp);
obj make_lambda(obj parameters, obj body);
obj begin_actions(obj exp);
obj cond2if(obj exp);
#endif
