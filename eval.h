#ifndef __EVAL_H__
#define __EVAL_H__
#include "type.h"
element *eval(element *exp, environment *env);
element *list_of_values(element *exps,environment *env);
element *eval_if(element *exp, environment *env);
element *eval_sequence(element *exps, environment *env);
element *eval_assignment(element *exp, environment *env);
element *eval_definition(element *exp, environment *env);
element *text_of_quotation(element *exp);
int is_number(element *exp);
int is_string(element *exp);
int is_self_evaluating(element *exp);
element *lambda_parameters(element *exp);
element *lambda_body(element *exp);
element *make_lambda(element *parameters, element *body);
element *begin_actions(element *exp);
int is_number(element *exp);
element *cond2if(element *exp);
#endif
