#include "apply.h"
#include "eval.h"
#include "type.h"
#include <stdio.h>
#include <stdlib.h>

obj apply(obj proc, obj args) {
    if (is_primitive_procedure(proc)) {
        return apply_primitive_procedure(proc, args);
    }
    else if (is_compound_procedure(proc)) {
        return eval_sequence(procedure_body(proc), extend_environment(procedure_parameters(proc), args, procedure_environment(proc)));
    }
    else {
        return NULL;
    }
}

int is_primitive_procedure(obj proc) {
    return is_tagged_list(proc,"primitive");
}
int is_compound_procedure(obj proc) {
    return is_tagged_list(proc,"procedure");
}

obj apply_primitive_procedure(obj proc, obj args) {
    // number of functions
    // stick to the limit of the register, only accept no more than 6 parameters?
    obj func = (obj)getContent(FUNC, proc);
    int argc = 0;
    table *tb = (table *)getContent(TABLE, args);
    table *arg[6];
    while (tb != NULL) {
        arg[argc] = args;
        argc++;
        args = cdr(args);
        tb = (table *)getContent(TABLE, args);
    }
    switch (argc) {
        case 0:
            return exec_zero(func);
        case 1:
            return exec_one(func, arg[0]);
        case 2:
            return exec_two(func, arg[0], arg[1]);
        case 3:
            return exec_three(func, arg[0], arg[1], arg[2]);
        case 4:
            return exec_three(func, arg[0], arg[1], arg[2], arg[3]);
        case 5:
            return exec_three(func, arg[0], arg[1], arg[2], arg[3], arg[4]);
        case 6:
            return exec_three(func, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);
        default:
            printf("Error: too much parameters for the function");
    }
}
    // apply(func, args[0], args[1], args[2])
obj eval_sequence(obj body, obj proc);
obj procedure_body(obj proc) {
    return car(cdr(cdr(proc)));
}
obj procedure_parameters(obj proc) {
    return car(cdr(proc));
}
obj procedure_environment(obj proc) {
    return car(cdr(cdr(cdr(proc))));
}

obj primitive_implementation(obj proc) {
    return car(cdr(proc));
}

obj make_procedure(obj para, obj body, obj env) {
    return cons(newTag("procedure"), cons(para, cons(body, cons(env, NULL))));
}
