#include <stdio.h>
#include <stdlib.h>
#include "cleaner.h"
#include "type.h"
void GCAdd(element *e, gc_element_list *head) {
    gc_element_list *next = head->next;
    head->next = malloc(sizeof(gc_element_list));
    head->e = e;
    head->next->next = next;
}
void cleanAll(gc_element_list *head) {
    gc_element_list *cur = head->next;
    while (cur != NULL) {
        destroyElement(cur->e);
        gc_element_list *next = cur->next;
        free(cur);
        cur = next;
    }
}
