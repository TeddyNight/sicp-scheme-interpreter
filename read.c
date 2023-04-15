#include "read.h"
#include "type.h"
#include "eval.h"
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

obj read() {
    return parse(input(), NULL);
}

obj input() {
    char *seq = malloc(1);
    *seq = '\0';
    obj sym = newSymbol(seq);
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

    // ) is for the end of main-clause or sub-clause
    // ' ' is for the start of sub-clause
    if (c == ')') {
        obj first = parse(str, NULL);
        if (rest == NULL)
            return parse(str, first);
        else
            return parse(str, cons(first, rest));
    }
    if (c == ' ') {
        return rest;
    }

    char *seq = malloc(1);
    *seq = '\0';
    obj res = newSymbol(seq);
    int noSym = 1;
    int isQuote = 0;
    while (c != '(' && c != ' ') {
        isQuote = 0;
        noSym = 0;
        if (c == '\'') {
            isQuote = 1;
            c = getReverseChar(str);
            continue;
        }
        putChar(res, c);
        c = getReverseChar(str);
    }

    if (!noSym) {
        reverseSym(res);
    }

    // Add Quote Tag
    if (isQuote) {
        res->type = STRING;
        res = cons(newTag("quote"), cons(res, NULL));
    }

    if (c == ' ') {
        return parse(str, cons(res, rest));
    }

    if (c == '(' && !noChar(str)) {
        //return parse(str, cons(cons(res, rest), NULL));
        return parse(str, cons(res, rest));
    }

    // parse finsh
    return cons(res, rest);
}
