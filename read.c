#include "read.h"
#include "type.h"
#include <stdio.h>
#include <stdlib.h>
/*
void parse(char *st, table *tb) {
    if (strncmp(sentence,"(",1) != 0) {
        tb->data = malloc(sizeof data);
        tb->data->type = ;
    }
}
*/

element *input() {
    element *sym = newSymbol(malloc(2),1);
    char c;
    while ((c = getchar()) != '\n') {
        putChar(sym,c);
    }
    return sym;
}

element *parse(element *str) {
    char c = getChar(str);
    if (c == '\0') {
        return NULL;
    }

    if (c == '(') {
        element *first = parse(str);
        element *rest = parse(str);
        return cons(first, rest);
    }

    if (c == '\0' || c == ')') {
        return NULL;
    }

    element* e = newSymbol(malloc(2),1);
    while (c != '\0' && c != ' ' && c != ')') {
        putChar(e, c);
        c = getChar(str);
    }

    return e;
}    
