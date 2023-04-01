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
void printTable(element *e) {
    if (e->type != TABLE) {
        return;
    }
    element *first = car(e);
    element *rest = cdr(e);

    if (first == NULL) {
        return;
    }
    else if (first->type == TABLE) {
        printTable(first);
    }
    else if (first->type == SYMBOL) {
        symbol *str = (symbol *)getContent(SYMBOL,first);
        printf("%s ",str->seq);
    }

    if (rest == NULL) {
        return;
    }
    if (rest->type == TABLE) {
        printTable(rest);
    }
    else if (rest->type == SYMBOL) {
        symbol *str = (symbol *)getContent(SYMBOL,rest);
        printf("%s ",str->seq);
    }
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
    if (sym == NULL) {
       return '\0';
    } 
    char res = getCharAt(sym->index,e);
    if (res != '\0')
        (sym->index)++;
    return res;
}
char getReverseChar(element *e) {
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
char putChar(element *e, char c) {
    symbol *sym = getContent(SYMBOL, e);
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
char getCharAt(int pos, element *e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL || pos > sym->length) {
        return '\0';
    }
    return *((sym->seq)+pos);
}
int noChar(element *e) {
    symbol *sym = getContent(SYMBOL, e);
    if (sym == NULL || sym->index >= (sym->length)) {
        return 1;
    }
    return 0;
}
void reverseSym(element *e) {
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
