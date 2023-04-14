#include "apply.h"
#include "eval.h"
#include "type.h"
#include "env.h"
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
    int argc = 0;
    obj arg[6];
    while (args != NULL) {
        arg[argc] = car(args);
        argc++;
        args = cdr(args);
    }
    return exec_func(primitive_procedure_implementation(proc), argc, arg);
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

obj primitive_procedure_implementation(obj proc) {
    return car(cdr(proc));
}

obj make_procedure(obj para, obj body, obj env) {
    return cons(newTag("procedure"), cons(para, cons(body, cons(env, NULL))));
}
