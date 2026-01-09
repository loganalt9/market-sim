#include "infra.h"
#include <stdlib.h>

order* create_order(int id, int size) {
    order* o = malloc(sizeof(order));
    o->id = id;
    o->size = size;
    o->next = NULL;

    return o;
}

void delete_order(price_level* level, int id) {
    order* curr = level->head;
    order* prev = level->head;
    while (curr) {
        if (curr->id == id) {
            prev->next = curr->next;
            free(curr);
            curr = NULL;
            break;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}