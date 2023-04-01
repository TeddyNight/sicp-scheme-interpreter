#ifndef __TYPE_H__
#define __TYPE_H__
enum TYPE {
    TABLE,
    SYMBOL
};
typedef struct ELEMENT {
    enum TYPE type;
    void *content;
} element;
typedef struct TABLE {
    element *first;
    element *rest;
} table;
typedef struct SYMBOL {
    char *seq;
    int index;
    int length;
    int size;
} symbol;

void* getContent(enum TYPE t, element *e); 

element *cons(element *first, element *rest);
element *car(element *tb);
element *cdr(element *tb);
void printTable(element *tb);

element* newSymbol(char *seq,int len);
char getChar(element *e); 
char putChar(element *e, char c);
char getCharAt(int pos, element *e); 
char getReverseChar(element *e);
int noChar(element *e);
void reverseSym(element *e);
#endif
