#include <stdio.h>
#include <stdlib.h>
#include "read.h"
#include "type.h"
#include "string.h"
#include "cleaner.h"
#include "eval.h"
extern gc_element_list e_list;

void testSymbol() {
    obj sym = read();
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
    while ((c = getReverseChar(sym)) != '\0') {
        printf("%c",c);
    }
    /*
    free(sym->seq);
    free(str);
    */
}

void testCons() {
    // (a b c)
    // (a (b c))
    // ((a b) (c d))
    obj sym = read();
    obj tb = parse(sym, NULL);
    /*
    symbol *first = (symbol *)getContent(SYMBOL,car(tb));
    printf("%s\n",first->seq);
    obj rest = cdr(tb);
    symbol *second = (symbol *)getContent(SYMBOL,car(rest));
    printf("%s",second->seq);
    rest = cdr(cdr(tb));
    symbol *third = (symbol *)getContent(SYMBOL,car(rest));
    printf("%s",third->seq);
    */
    printTable(tb);
    /*
    free(sym->seq);
    free(str);
    */
}
/*
void testList() {
    obj e = list(newTag("abc"), newTag("def"), newTag("ghi"));
    printTable(e);
}
*/

int main() {
    //testSymbol();
    //testCons();
    testList();
    cleanAll(&e_list);
}
