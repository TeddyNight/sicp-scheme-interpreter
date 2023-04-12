#ifndef __ENV_H__
#define __ENV_H__
#include "type.h"
obj lookup_variable_value(obj var, obj env);
obj lookup_variable_scan(obj var, obj vars, obj vals);
obj extend_environment(obj vars, obj vals, obj base_env);
obj define_variable_scan(obj var, obj val, obj vars, obj vals);
obj define_variable(obj var, obj val, obj env);
obj set_variable_scan(obj var, obj val, obj vars, obj vals);
obj set_variable_value(obj var, obj val, obj env);

obj enclosing_environment(obj env);
obj first_frame(obj env);
obj the_empty_environment();
int is_empty_environment(obj env);
obj make_frame(obj vars, obj vals);
obj frame_variables(obj frame);
obj frame_values(obj frame);
obj add_binding_to_frame(obj var, obj val, obj frame);

obj primitive_procedure_names();
obj primitive_procedure_objects();
obj setup_environment();
#endif
