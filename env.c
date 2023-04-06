#include "env.h"
#include "type.h"
#include <stdio.h>

obj lookup_variable_scan(obj var, obj vars, obj vals) {
    if (vars == NULL) {
        return NULL;
    }
    else if (eq(var, car(vars))) {
        return car(vals);
    }
    else {
        return lookup_variable_scan(var, cdr(vars), cdr(vals));
    }
}
obj lookup_variable_value(obj var, obj env) {
    if (is_empty_environment(env)) {
        //TODO Error: Unbound variable
        return NULL;
    }
    obj frame = first_frame(env);
    obj res = lookup_variable_scan(var, frame_variables(frame), frame_values(frame));
    if (res == NULL) {
        res = lookup_variable_value(var, enclosing_environment(env));
    }
    return res;
}
obj extend_environment(obj vars, obj vals, obj base_env) {
    if (length(vars) == length(vals)) {
        return cons(make_frame(vars,vals), base_env);
    }
    else if (length(vars) < length(vals)) {
        // TODO Error: Too many arguments supplied
        return NULL;
    }
    else {
        // TODO Error: Too few arguments supplied 
        return NULL;
    }
}
obj define_variable_scan(obj var, obj val, obj vars, obj vals) {
    if (vars == NULL) {
        return NULL;
    }
    else if (eq(var, car(vars))) {
        return set_car(vals, val);
    }
    else {
        return define_variable_scan(var, val, cdr(vars), cdr(vals));
    }
}
obj define_variable(obj var, obj val, obj env) {
    obj frame = first_frame(env);
    obj res = define_variable_scan(var, val, frame_variables(frame), frame_values(frame));
    if (res == NULL) {
        return add_binding_to_frame(var, val, frame);
    }
    return res;
}
obj set_variable_scan(obj var, obj val, obj vars, obj vals) {
    if (vars == NULL) {
        return NULL;
    }
    else if (eq(var, car(vars))) {
        return set_car(vals, val);
    }
    else {
        return set_variable_scan(var, val, cdr(vars), cdr(vals));
    }
}
obj set_variable_value(obj var, obj val, obj env) {
    if (is_empty_environment(env)) {
        //TODO error Unbound variable -- SET!
        return NULL;
    }
    obj frame = first_frame(env);
    obj res = set_variable_scan(var, val, frame_variables(frame), frame_values(frame));
    if (res == NULL) {
        return set_variable_value(var, val, enclosing_environment(env));
    }
    else {
        return res;
    }
}

obj enclosing_environment(obj env) {
    return cdr(env);
}
obj first_frame(obj env) {
    return car(env);
}
obj the_empty_environment() {
    return cons(NULL,NULL);
}
int is_empty_environment(obj env) {
    table *tb = (table *)getContent(TABLE, env);
    if (tb == NULL) {
        return 0;
    }
    else if (tb->first == NULL && tb->rest == NULL) {
        return 1;
    }
    else {
        return 0;
    }
}
obj make_frame(obj vars, obj vals) {
    return cons(vars, vals);
}
obj frame_variables(obj frame) {
    return car(frame);
}
obj frame_values(obj frame) {
    return cdr(frame);
}
obj add_binding_to_frame(obj var, obj val, obj frame) {
    set_car(frame, cons(var, car(frame)));
    return set_cdr(frame, cons(val, cdr(frame)));
}