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
    element *sym = newSymbol(malloc(1),0);
    char c;
    while ((c = getchar()) != '\n') {
        putChar(sym,c);
    }
    return sym;
}

element *parse(element *str, element *rest) {
    // TODO: improve the way to parse the experssion
    char c;
    c = getReverseChar(str);
    if (c == '\0') {
        return rest;
    }

    // main-clause or sub-clause
    if (c == ')') {
        return parse(str, rest);
    }

    element *res = newSymbol(malloc(1),0);
    int noSym = 1;
    while (c != '(' && c != ' ') {
        noSym = 0;
        putChar(res, c);
        c = getReverseChar(str);
    }

    if (c == ' ') {
        return parse(str, cons(res, rest));
    }

    if (c == '(' && !noChar(str)) {
        return parse(str, cons(cons(res, rest), NULL));
    }

    // parse finsh
    if (noSym)
        return rest;
    else {
        reverseSym(res);
        return cons(res, rest);
    }
}
