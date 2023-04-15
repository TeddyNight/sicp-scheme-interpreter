#include "type.h"
#include "cleaner.h"
#include "apply.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
gc_element_list e_list; 
void* getContent(enum TYPE t, obj e) {
    if (e == NULL) return NULL;
    if (e->type == t) return e->content;
    else return NULL;
}
obj newElement(enum TYPE t, void *c) {
    obj e = malloc(sizeof(element));
    e->type = t;
    e->content = c;
    GCAdd(e,&e_list);
    return e;
}
void destroyElement(obj e) {
    if (e == NULL)
        return;
    if (e->type == SYMBOL)
        destroySym(e);
    else if (e->type == TABLE)
        destroyTable(e);
    free(e);
}
void destroySym(obj e) {
    if (e == NULL)
        return;
    symbol *sym = (symbol *)getContent(SYMBOL, e);
    if (sym == NULL)
        return;
    free(sym->seq);
    free(sym);
}
void destroyTable(obj e) {
    if (e == NULL)
        return;
    table *tb = (table *)getContent(TABLE, e);
    if (tb == NULL)
        return;
    destroyElement(tb->first);
    destroyElement(tb->rest);
    free(tb);
}

obj cons(obj first, obj rest) {
    table* tb = (table *)malloc(sizeof(table));
    tb->first = (void *)first;
    tb->rest = (void *)rest;
    return newElement(TABLE, tb);
}
obj car(obj e) {
    table *tb = (table *)getContent(TABLE,e);
    if (tb == NULL) return NULL;
    return tb->first;
}
obj cdr(obj e) {
    table *tb = (table *)getContent(TABLE,e);
    if (tb == NULL) return NULL;
    return tb->rest;
}
obj cadr(obj tb) {
    return car(cdr(tb));
}
/*
obj list(obj arg1, ...) {
    va_list ap;
    obj t;

    va_start(ap, arg1);
    obj res = cons(arg1, NULL);
    obj e = res;
    while ((t = va_arg(ap, obj)) != NULL) {
        set_cdr(e, cons(t, NULL));
        e = cdr(e);
    }
    va_end(ap);
    return res;
}
*/
    
void printTable(obj e) {
    if (e->type != TABLE) {
        return;
    }
    obj first = car(e);
    obj rest = cdr(e);

    if (first == NULL) {
        return;
    }
    else {
        user_print(first);
    }

    if (rest == NULL) {
        return;
    }
    user_print(rest);
}
int is_pair(obj exp) {
    if (car(exp) && cdr(exp) != NULL) {
        return 1;
    }
    return 0;
}
int length(obj exp) {
    table *tb = (table *)getContent(TABLE, exp);
    if (tb == NULL) {
        return 0;
    }
    else if (car(exp) == NULL) {
        return 0;
    }
    else {
        return 1 + length(cdr(exp));
    }
}
obj set_car(obj tb, obj e) {
    table *res = (table *)getContent(TABLE, tb);
    if (res == NULL)
        return NULL;
    res->first = e;
    return tb;
}
obj set_cdr(obj tb, obj e) {
    table *res = (table *)getContent(TABLE, tb);
    if (res == NULL)
        return NULL;
    res->rest = e;
    return tb;
}

obj newSymbol(char *seq) {
    int len = strlen(seq);
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym->seq = seq;
    sym->index = 0;
    sym->length = len;
    sym->size = len+1;
    return newElement(SYMBOL, sym);
}
char getChar(obj e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL) {
       return '\0';
    } 
    char res = getCharAt(sym->index,e);
    if (res != '\0')
        (sym->index)++;
    return res;
}
char getReverseChar(obj e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL) {
       return '\0';
    }
    int pos = (sym->length)-1-(sym->index);
    if (pos < 0)
        return '\0';
    char res = getCharAt(pos,e);
    (sym->index)++;
    return res;
}
char putChar(obj e, char c) {
    symbol *sym = (symbol *)getContent(SYMBOL, e);
    if (sym == NULL) {
        return '\0';
    }
    if ((sym->length)+1 >= sym->size) {
        char *temp = (char *)malloc((int)(1.75*(sym->size)));
        strncpy(temp,sym->seq,sym->length);
        sym->size = (int)(1.75*(sym->size));
        free(sym->seq);
        sym->seq = temp;
    }
    *((sym->seq)+(sym->length)) = c;
    *((sym->seq)+(sym->length)+1) = '\0';
    (sym->length)++;

    return *((sym->seq)+(sym->index));
}
void putString(obj e,char *c) {
    while (c != NULL && *c != '\0') {
        putChar(e,*c);
        c++;
    }
}
char getCharAt(int pos, obj e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL || pos > sym->length) {
        return '\0';
    }
    return *((sym->seq)+pos);
}
int noChar(obj e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL || sym->index >= (sym->length)) {
        return 1;
    }
    return 0;
}
void reverseSym(obj e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL)
        return;
    char *seq = sym->seq;
    int len = sym->length;
    for (int i = 0; i < len/2; i++) {
        char t = seq[i];
        seq[i] = seq[len-1-i];
        seq[len-1-i] = t;
    }
}

int is_true(obj e) {
    obj val = (obj )getContent(BOOLEAN,e);
    if (val->content) {
        return 1;
    }
    else {
        return 0;
    }
}
int is_false(obj e) {
    obj val = (obj )getContent(BOOLEAN,e);
    if (!(val->content)) {
        return 1;
    }
    else {
        return 0;
    }
}

int eq_symbol(obj x,obj y) {
    symbol *a = (symbol *)getContent(SYMBOL, x);
    symbol *b = (symbol *)getContent(SYMBOL, y);
    if (a == NULL || b == NULL) {
        return 0;
    }
    else if (strcmp(a->seq,b->seq) == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int eq_table(obj x,obj y) {
    table *a = (table *)getContent(TABLE, x);
    table *b = (table *)getContent(TABLE, y);
    if (a == NULL || b == NULL) {
        return 0;
    }
    else if (eq(car(x), car(y)) && eq(cdr(x), cdr(y))) {
        return 1;
    }
    else {
        return 0;
    }
}

int eq(obj x, obj y) {
    //TODO the original definition of this is to compare the address of two elements
    if (x == NULL && y == NULL) {
        return 1;
    }
    else if (x->type != y->type) {
        return 0;
    }
    else if (eq_symbol(x, y)) {
        return 1;
    }
    else if (eq_table(x, y)) {
        return 1;
    }
    else {
        return 0;
    }
}

void printSymbol(obj o) {
    symbol *sym = (symbol *)getContent(SYMBOL, o);
    if (sym == NULL) {
        return;
    }
    printf("%s\n",sym->seq);
}
void printString(obj o) {
    string *str = (string *)getContent(STRING, o);
    if (str == NULL) {
        return;
    }
    printf("%s\n",str->seq);
}
obj user_print(obj o) {
    if (o == NULL)
        return NULL;
    table *tb = (table *)getContent(TABLE, o);
    if (tb != NULL) {
        if (is_tagged_list(o, "quote")) {
            printSymbol(car(cdr(o)));
        }
        else if (is_compound_procedure(o)) {
            printf("!!compound-procedure\nprocedure_parameters:\n");
            user_print(procedure_parameters(o));
            printf("procedure_body:\n");
            user_print(procedure_body(o));
            printf("<procedure-env>\n");
        }
        else {
            printTable(o);
        }
        return NULL;
    }
    if (is_symbol(o)) {
        printSymbol(o);
    }
    if (is_string(o)) {
        printString(o);
    }
    void *boolean = getContent(BOOLEAN, o);
    if (boolean != NULL) {
        if (o->content) {
            printf("True\n");
        }
        else {
            printf("False\n");
        }
    }
}

obj newTag(char *tag) {
    char *seq = malloc(1);
    *seq = '\0';
    obj res = newSymbol(seq);
    putString(res,tag);
    return res;
}

// Function pointers aren't necessarily the same size as regular pointers
// avoid this undefined behavior
// maybe the pointer should be put in (list 'primitive <pointer>) ?
obj newFunction(int argc, obj (*func)()) {
    function *func_o = malloc(sizeof(function));
    func_o->argc = argc;
    func_o->func = func;
    return newElement(FUNCTION, func_o);
}

obj exec_func(obj proc, int argc, obj *arg) {
    obj (*func)() = ((function *)(getContent(FUNCTION, proc)))->func;
    switch (argc) {
        case 0:
            return func();
        case 1:
            return ((obj (*)(obj))func)(arg[0]);
        case 2:
            return ((obj (*)(obj, obj))func)(arg[0], arg[1]);
        case 3:
            return ((obj (*)(obj, obj, obj))func)(arg[0], arg[1], arg[2]);
        case 4:
            return ((obj (*)(obj, obj, obj ,obj))func)(arg[0], arg[1], arg[2], arg[3]);
        case 5:
            return ((obj (*)(obj, obj, obj ,obj, obj))func)(arg[0], arg[1], arg[2], arg[3], arg[4]);
        case 6:
            return ((obj (*)(obj, obj, obj ,obj, obj, obj))func)(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);
        default:
            printf("Error: too much parameters for the function");
    }
    return NULL;
}    

obj map(obj proc, obj list) {
    function *func = getContent(FUNCTION, proc);
    if (func == NULL || func->argc != 1)
        return NULL;
    obj cur = car(list);
    obj rest = cdr(list);
    if (cur == NULL)
        return NULL;
    return cons(exec_func(proc, 1, &cur), map(proc, rest));
} 

obj display(obj o) {
    return o;
}
