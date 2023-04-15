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
        obj body = procedure_body(proc);
        obj env = extend_environment(procedure_parameters(proc), args, procedure_environment(proc));
        return eval_sequence(body, env);
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
    static obj arg[6];
    while (args != NULL) {
        arg[argc] = car(args);
        argc++;
        args = cdr(args);
    }
    return exec_func(primitive_procedure_implementation(proc), argc, arg);
}
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
    // only one exp does not form the seq of exps
    // have to form it manually?
    if (body != NULL && car(body) != TABLE) {
        body = cons(body, NULL);
    }
    // env cause loop here
    // because the env stores it & the env it contains are same
    return cons(newTag("procedure"), cons(para, cons(body, cons(env, NULL))));
}
