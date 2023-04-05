#ifndef __TYPE_H__
#define __TYPE_H__
enum TYPE {
    TABLE,
    SYMBOL,
    BOOLEAN
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
typedef struct ENVIRONMENT {
} environment;

void* getContent(enum TYPE t, element *e); 
void destroyElement(element *e);
element *newElement(enum TYPE t, void *c); 

element *cons(element *first, element *rest);
element *car(element *tb);
element *cdr(element *tb);
void printTable(element *tb);
void destroyTable(element *tb);
int is_pair(element *exp);

element* newSymbol(char *seq,int len);
char getChar(element *e); 
char putChar(element *e, char c);
char getCharAt(int pos, element *e); 
char getReverseChar(element *e);
int noChar(element *e);
void reverseSym(element *e);
void destroySym(element *e);

int is_true(element *e);
int is_false(element *e);
#endif
