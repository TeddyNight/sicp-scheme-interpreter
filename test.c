#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "type.h"
#include "string.h"
void testSymbol() {
    element* sym = input();
    char c;
    putChar(sym,'!');
    putChar(sym,'!');
    putChar(sym,'!');
    putChar(sym,'!');

    putChar(sym,'!');
    putChar(sym,'!');
    putChar(sym,'!');
    putChar(sym,'!');

    putChar(sym,'!');
    while ((c = getChar(sym)) != '\0') {
        printf("%c",c);
    }
    /*
    free(sym->seq);
    free(str);
    */
}

void testCons() {
    // (a b c)
    element *sym = input();
    element *tb = parse(sym);
    symbol *first = (symbol *)getContent(SYMBOL,car(tb));
    printf("%s",first->seq);
    table *rest = (table *)getContent(SYMBOL,cdr(tb));
    printf("%d",rest->first->type);
    /*
    free(sym->seq);
    free(str);
    */
}

int main() {
    //testSymbol();
    testCons();
}
