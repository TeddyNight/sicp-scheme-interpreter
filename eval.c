#include "eval.h"
#include "apply.h"
#include "type.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
obj eval(obj exp, obj env) {
    obj res = NULL;
    if (is_self_evaluating(exp)) {
        res = exp;
    }
    else if (is_variable(exp)) {
        res = lookup_variable_value(exp, env);
    }
    else if (is_quoted(exp)) {
        res = text_of_quotation(exp);
    }
    else if (is_assignment(exp)) {
        res = eval_assignment(exp, env);
    }
    else if (is_definition(exp)) {
        res = eval_definition(exp, env);
    }
    else if (is_if(exp)) {
        res = eval_if(exp, env);
    }
    else if (is_lambda(exp)) {
        res = make_procedure(lambda_parameters(exp), lambda_body(exp), env);
    }
    else if (is_begin(exp)) {
        res = eval_sequence(begin_actions(exp), env);
    }
    else if (is_cond(exp)) {
        res = eval(cond2if(exp), env);
    }
    else if (is_application(exp)) {
        obj proc = eval(car(exp), env);
        obj para = list_of_values(cdr(exp), env);
        res = apply(proc, para);
    }
    return res;
}

obj list_of_values(obj exps, obj env) {
    if (getContent(TABLE,exps) == NULL) {
        return NULL;
    }
    return cons(eval(car(exps), env), list_of_values(cdr(exps), env));
}

obj eval_if(obj exp, obj env) {
    if (is_true(eval(car(exp),env))) {
        return eval(car(cdr(exp)), env);
    }
    else {
        return eval(car(cdr(cdr(exp))), env);
    }
}

obj eval_sequence(obj exps,obj env) {
    if (cdr(exps) == NULL) {
        return eval(car(exps), env);
    }
    else {
        //TODO eval sequence here should not reach to NULL and return
        eval(car(exps), env);
        return eval_sequence(cdr(exps), env);
    }
}

obj eval_assignment(obj exp, obj env) {
    set_variable_value(car(exp), eval(cdr(exp), env), env);
    return NULL;
}

obj eval_definition(obj exp, obj env) {
    obj var = definition_variable(exp);
    obj val = definition_value(exp);
    define_variable(var, eval(val, env), env);
}

int is_number(obj exp) {
    symbol *sym = (symbol *)getContent(SYMBOL,exp);
    if (sym == NULL)
        return 0;
    char c;
    while ((c = getChar(exp)) != '\0') {
        if (c < '0' || c > '9')
            return 0;
    }
    sym->index = 0;
    return 1;
}

int is_tagged_list(obj exp, char *t) {
    table *tb = (table *)getContent(TABLE,exp);
    if (tb == NULL)
        return 0;
    symbol *tag = (symbol *)getContent(SYMBOL,car(exp));
    if (tag == NULL)
        return 0;
    if (strcmp(tag->seq,t) == 0)
        return 1;
    return 0;
}

int is_quoted(obj exp) {
    return is_tagged_list(exp, "quote");
}

obj text_of_quotation(obj exp) {
    return car(cdr(exp));
}

int is_string(obj exp) {
    return (getContent(STRING, exp) != NULL);
}

int is_assignment(obj exp) {
    return is_tagged_list(exp,"set!");
}

int is_definition(obj exp) {
    return is_tagged_list(exp,"define");
}

int is_self_evaluating(obj exp) {
    if (is_number(exp))
        return 1;
    else if(is_string(exp))
        return 1;
    else
        return 0;
}

int is_symbol(obj exp) {
    symbol *sym = (symbol *)getContent(SYMBOL,exp);
    if (sym == NULL)
        return 0;

    // Keywords..
    if (strcmp(sym->seq,"if") == 0)
        return 0;
    if (strcmp(sym->seq,"cond") == 0)
        return 0;
    if (strcmp(sym->seq,"set!") == 0)
        return 0;
    if (strcmp(sym->seq,"define") == 0)
        return 0;
    if (strcmp(sym->seq,"begin") == 0)
        return 0;

    return 1;
}

int is_variable(obj exp) {
    return is_symbol(exp);
}

obj definition_variable(obj exp) {
    if (is_symbol(car(cdr(exp)))) {
        return car(cdr(exp));
    }
    else {
        return car(car(cdr(exp)));
    }
}

obj definition_value(obj exp) {
    if (is_symbol(car(cdr(exp)))) {
        return car(cdr(cdr(exp)));
    }
    else {
        return make_lambda(cdr(car(cdr(exp))),cdr(cdr(exp)));
    }
}

int is_lambda(obj exp) {
    return is_tagged_list(exp,"lambda");
}

obj lambda_parameters(obj exp) {
    return car(cdr(exp));
}

obj lambda_body(obj exp) {
    return cdr(cdr(exp));
}

obj make_lambda(obj parameters, obj body) {
    return cons(newTag("lambda"), cons(parameters, cons(body, NULL)));
}

int is_if(obj exp) {
    return is_tagged_list(exp,"if");
}

obj if_predicate(obj exp) {
    return car(cdr(exp));
}

obj if_consequent(obj exp) {
    return car(cdr(cdr(exp)));
}

obj if_alternative(obj exp) {
    if (cdr(cdr(cdr(exp))) != NULL) {
        return car(cdr(cdr(cdr(exp))));
    }
    else {
        return newElement(BOOLEAN, 0);
    }
}

obj make_if(obj predicate, obj consequent, obj alternative) {
    return cons(newTag("if"), cons(predicate, cons(consequent, cons(alternative, NULL))));
}

int is_begin(obj exp) {
    return is_tagged_list(exp,"begin");
}

obj begin_actions(obj exp) {
    return cdr(exp);
}

int is_last_exp(obj seq) {
    return (cdr(seq) == NULL);
}

obj first_exp(obj seq) {
    return car(seq);
}

obj rest_exps(obj seq) {
    return cdr(seq);
}

obj make_begin(obj seq) {
    return cons(newTag("begin"), cons(seq, NULL));
}

obj sequence2exp(obj seq) {
    if (seq == NULL) {
        return seq;
    }
    else if(is_last_exp(seq)) {
        return first_exp(seq);
    }
    else {
        make_begin(seq);
    }
}

int is_application(obj exp) {
    return is_pair(exp);
}

int is_cond(obj exp) {
    return is_tagged_list(exp,"cond");
}

obj cond_clauses(obj exp) {
    return cdr(exp);
}

obj cond_predicate(obj clause) {
    return car(clause);
}

obj cond_actions(obj clause) {
    return cdr(clause);
}

obj is_cond_else_clause(obj clause) {
    symbol *sym = (symbol *)getContent(SYMBOL, cond_predicate(clause));
    if (strcmp(sym->seq, "else")) {
        return 1;
    }
    else {
        return 0;
    }
}

obj expand_clauses(obj clauses) {
    if (clauses == NULL) {
        return newElement(BOOLEAN,0);
    }
    obj first = car(clauses);
    obj rest = cdr(clauses);
    if (is_cond_else_clause(first)) {
        if (rest == NULL) {
            return sequence2exp(cond_actions(first));
        }
        else {
            // TODO: Error Handling: ELSE clause isn't last -- CONF->IF CLAUSES
            return NULL;
        }
    }
    else {
        return make_if(cond_predicate(first), sequence2exp(cond_actions(first)),
                expand_clauses(rest));
    }
}

obj cond2if(obj exp) {
    return expand_clauses(cond_clauses(exp));
}
