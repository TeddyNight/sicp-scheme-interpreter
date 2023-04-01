#ifndef __CLEANER_H__
#define __CLEANER_H__
#include "type.h"
typedef struct GC_ELEMENT_LIST {
    element *e;
    struct GC_ELEMENT_LIST *next;
} gc_element_list;
void GCAdd(element *e, gc_element_list *head);
void cleanAll(gc_element_list *head);
#endif
