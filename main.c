#include <stdio.h>
#include <stdio.h>
#include "type.h"
#include "cleaner.h"
#include "eval.h"
#include "apply.h"
#include "read.h"
#include "env.h"

#define INPUT_PROMPT ";;;M-Eval input:\n"
#define OUTPUT_PROMPT ";;;M-Eval value:\n"

obj primitive_procedures;
int main() {
    obj the_global_environment = setup_environment();
    // driver loop
    while (1) {

        printf(INPUT_PROMPT);
        obj input = read();
        obj output = eval(input, the_global_environment);

        printf(OUTPUT_PROMPT);
        user_print(output);
    }
}
