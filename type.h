#ifndef __TYPE_H__
#define __TYPE_H__
enum TYPE {
    TABLE,
    SYMBOL,
    BOOLEAN,
    FUNCTION
};
typedef struct ELEMENT {
    enum TYPE type;
    void *content;
} element;
typedef element* obj;
typedef struct TABLE {
    obj first;
    obj rest;
} table;
typedef struct SYMBOL {
    char *seq;
    int index;
    int length;
    int size;
} symbol;
/*
typedef struct FUNCTION {
    int argc;
    union {
*/      

void* getContent(enum TYPE t, obj e); 
void destroyElement(obj e);
obj newElement(enum TYPE t, void *c); 

obj cons(obj first, obj rest);
obj car(obj tb);
obj cdr(obj tb);
obj set_car(obj tb, obj e);
obj set_cdr(obj tb, obj e);
void printTable(obj tb);
void destroyTable(obj tb);
int is_pair(obj exp);
int length(obj exp);

obj newSymbol(char *seq,int len);
char getChar(obj e); 
char putChar(obj e, char c);
char getCharAt(int pos, obj e); 
char getReverseChar(obj e);
int noChar(obj e);
void reverseSym(obj e);
void destroySym(obj e);

int is_true(obj e);
int is_false(obj e);
int eq(obj x, obj y);
#endif
