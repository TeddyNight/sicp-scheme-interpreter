#include "type.h"
#include "cleaner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
gc_element_list e_list; 
void* getContent(enum TYPE t, obj e) {
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
    tb->first = first;
    tb->rest = rest;
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
void printTable(obj e) {
    if (e->type != TABLE) {
        return;
    }
    obj first = car(e);
    obj rest = cdr(e);

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
int is_pair(obj exp) {
    if (car(exp) && (getContent(SYMBOL,cdr(exp)) || cdr(cdr(exp)) == NULL)) {
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

obj newSymbol(char *seq, int len) {
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
