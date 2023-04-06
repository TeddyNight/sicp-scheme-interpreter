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

obj input() {
    obj sym = newSymbol(malloc(1),0);
    char c;
    while ((c = getchar()) != '\n') {
        putChar(sym,c);
    }
    return sym;
}

obj parse(obj str, obj rest) {
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

    obj res = newSymbol(malloc(1),0);
    int noSym = 1;
    int isQuote = 0;
    if (c == '\'') {
        isQuote = 1;
        c = getReverseChar(str);
    }
    while (c != '(' && c != ' ') {
        noSym = 0;
        putChar(res, c);
        c = getReverseChar(str);
    }

    // Add Quote Tag
    if (isQuote) {
        obj tag = newSymbol(malloc(1),0);
        putString(tag,"quote");
        res = cons(tag,res);
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
