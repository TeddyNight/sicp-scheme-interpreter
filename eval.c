#include "eval.h"
#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
element *eval(element *exp, environment *env) {
    element *res = NULL;
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
        res = apply(eval(car(exp), env), list_of_values(cdr(exp), env));
    }
    return res;
}

element *list_of_values(element *exps, environment *env) {
    if (getContent(TABLE,exps) == NULL) {
        return NULL;
    }
    cons(eval(car(exps), env), list_of_values(cdr(exps), env));
}

element *eval_if(element *exp, environment *env) {
    if (is_true(eval(car(exp),env))) {
        return eval(car(cdr(exp)), env);
    }
    else {
        return eval(car(cdr(cdr(exp))), env);
    }
}

element *eval_sequence(element *exps,environment *env) {
    if (cdr(exps) == NULL) {
        return eval(car(exps), env);
    }
    else {
        eval(car(exps), env);
        eval_sequence(cdr(exps), env);
    }
}

element *eval_assignment(element *exp, environment *env) {
    set_variable_value(car(exp), eval(cdr(exp), env));
    return NULL;
}

element *eval_definition(element *exp, environment *env) {
    define_variable(car(exp), eval(cdr(exp), env));
}

int is_number(element *exp) {
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

int is_tagged_list(element *exp, char *t) {
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

int is_quoted(element *exp) {
    return is_tagged_list(exp, "quote");
}

element *text_of_quotation(element *exp) {
    return car(cdr(exp));
}

int is_string(element *exp) {
    return is_tagged_list(exp, "quote");
}

int is_assignment(element *exp) {
    return is_tagged_list(exp,"set!");
}

int is_definition(element *exp) {
    return is_tagged_list(exp,"define");
}

int is_self_evaluating(element *exp) {
    if (is_number(exp))
        return 1;
    else if(is_string(exp))
        return 1;
    else
        return 0;
}

int is_symbol(element *exp) {
    symbol *sym = (symbol *)getContent(SYMBOL,exp);
    if (sym == NULL)
        return 0;

    // Keywords..
    if (strcmp(sym->seq,"if"))
        return 0;
    if (strcmp(sym->seq,"cond"))
        return 0;
    if (strcmp(sym->seq,"set!"))
        return 0;
    if (strcmp(sym->seq,"define"))
        return 0;
    if (strcmp(sym->seq,"begin"))
        return 0;

    return 1;
}

int is_variable(element *exp) {
    return is_symbol(exp);
}

element *definition_variable(element *exp) {
    if (is_symbol(car(cdr(exp)))) {
        return car(cdr(exp));
    }
    else {
        return car(car(cdr(exp)));
    }
}

element *definition_value(element *exp) {
    if (is_symbol(car(cdr(exp)))) {
        return car(cdr(cdr(exp)));
    }
    else {
        return make_lambda(cdr(car(cdr(exp))),cdr(cdr(exp)));
    }
}

element *newTag(char *tag) {
    symbol *res = newSymbol(malloc(1),0);
    putString(res,tag);
    return res;
}

int is_lambda(element *exp) {
    return is_tagged_list(exp,"lambda");
}

element *lambda_parameters(element *exp) {
    return car(cdr(exp));
}

element *lambda_body(element *exp) {
    return cdr(cdr(exp));
}

element *make_lambda(element *parameters, element *body) {
    return cons(newTag("lambda"), cons(parameters,body));
}

int is_if(element *exp) {
    return is_tagged_list(exp,"if");
}

int if_predicate(element *exp) {
    return car(cdr(exp));
}

int if_consequent(element *exp) {
    return car(cdr(cdr(exp)));
}

element *if_alternative(element *exp) {
    if (cdr(cdr(cdr(exp))) != NULL) {
        return car(cdr(cdr(cdr(exp))));
    }
    else {
        return newElement(BOOLEAN, 0);
    }
}

element *make_if(element *predicate, element *consequent, element *alternative) {
    return cons(newTag("if"), cons(predicate, cons(consequent, alternative)));
}

int is_begin(element *exp) {
    return is_tagged_list(exp,"begin");
}

element *begin_actions(element *exp) {
    return cdr(exp);
}

int is_last_exp(element *seq) {
    return (cdr(seq) == NULL);
}

element *first_exp(element *seq) {
    return car(seq);
}

element *rest_exps(element *seq) {
    return cdr(seq);
}

element *make_begin(element *seq) {
    return cons(newTag("begin"), seq);
}

element *sequence2exp(element *seq) {
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

int is_application(element *exp) {
    return is_pair(exp);
}

int is_cond(element *exp) {
    return is_tagged_list(exp,"cond");
}

element *cond_clauses(element *exp) {
    return cdr(exp);
}

element *cond_predicate(element *clause) {
    return car(clause);
}

element *cond_actions(element *clause) {
    return cdr(clause);
}

element *is_cond_else_clause(element *clause) {
    symbol *sym = (symbol *)getContent(SYMBOL, cond_predicate(clause));
    if (strcmp(sym->seq, "else")) {
        return 1;
    }
    else {
        return 0;
    }
}

element *expand_clauses(element *clauses) {
    if (clauses == NULL) {
        return newElement(BOOLEAN,0);
    }
    element *first = car(clauses);
    element *rest = cdr(clauses);
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

element *cond2if(element *exp) {
    return expand_clauses(cond_clauses(exp));
}
