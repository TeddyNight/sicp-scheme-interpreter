#include "type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void* getContent(enum TYPE t, element *e) {
    if (e->type == t) return e->content;
    else return NULL;
}

element *cons(element *first, element *rest) {
    element *res = (element *)malloc(sizeof(element));
    res->type = TABLE;
    table* tb = (table *)malloc(sizeof(table));
    res->content = tb;
    tb->first = first;
    tb->rest = rest;
    return res;
}
element *car(element *e) {
    table *tb = (table *)getContent(TABLE,e);
    if (tb == NULL) return NULL;
    return tb->first;
}
element *cdr(element *e) {
    table *tb = (table *)getContent(TABLE,e);
    if (tb == NULL) return NULL;
    return tb->rest;
}

element* newSymbol(char *seq, int len) {
    symbol *sym = (symbol *)malloc(sizeof(symbol));
    sym->seq = seq;
    sym->index = 0;
    sym->length = len;
    sym->size = len+1;
    element *res = (element *)malloc(sizeof(element));
    res->type = SYMBOL;
    res->content = (void *)sym;
    return res;
}
char getChar(element *e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL || sym->index > sym->length) {
        return '\0';
    }
    return *((sym->seq)+(sym->index++));
}
char putChar(element *e, char c) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL) {
        return '\0';
    }
    if (sym->length >= sym->size) {
        char *temp = (char *)malloc((int)(1.75*(sym->size)));
        strncpy(temp,sym->seq,sym->length);
        sym->size = (int)(1.75*(sym->size));
        free(sym->seq);
        sym->seq = temp;
    }
    *((sym->seq)+(sym->length)-1) = c;
    *((sym->seq)+(sym->length)) = '\0';
    (sym->length)++;

    return *((sym->seq)+(sym->index));
}
