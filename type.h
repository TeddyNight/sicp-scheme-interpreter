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
// TODO may be all parameters should be put in stack first, 
// then taken out using helper function
typedef struct FUNCTION {
    int argc;
    obj (*func)();
} function;

void* getContent(enum TYPE t, obj e); 
void destroyElement(obj e);
obj newElement(enum TYPE t, void *c); 

obj cons(obj first, obj rest);
obj car(obj tb);
obj cdr(obj tb);
obj cadr(obj tb);
obj set_car(obj tb, obj e);
obj set_cdr(obj tb, obj e);
void printTable(obj tb);
void destroyTable(obj tb);
int is_pair(obj exp);
int length(obj exp);
obj list(obj arg1,...);

obj newSymbol(char *seq);
char getChar(obj e); 
char putChar(obj e, char c);
void putString(obj e, char *tag);
char getCharAt(int pos, obj e); 
char getReverseChar(obj e);
int noChar(obj e);
void reverseSym(obj e);
void destroySym(obj e);
void printSymbol(obj o);

obj newFunction(int argc, obj (*func)());
obj exec_func(obj proc, int argc, obj *arg); 

obj newTag(char *tag); 

int is_true(obj e);
int is_false(obj e);
int eq(obj x, obj y);
obj map(obj proc, obj list);
#endif
