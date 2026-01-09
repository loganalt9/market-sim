#include "infra.h"
#include <stdlib.h>

price_level* create_price_level(double price) {
    price_level* pl = malloc(sizeof(price_level));
    pl->price = price;

    return pl;
}

void delete_price_level(price_level* pl) {
    order* curr = pl->head;

    while(curr) {
        order* tmp = curr->next;
        free(curr);
        curr = tmp;
    }

    free(pl);
    pl = NULL;
}

void add_order_price_level(price_level* pl, order* o) {
    if (pl->tail) {
        pl->tail->next = o;
        pl->tail = 0;
    } else {
        pl->head = o;
        pl->tail = o;
    }
}

order* remove_order_price_level(price_level* pl) {
    if (pl->head) {
        order* ret = pl->head;
        pl->head = ret->next;
        ret->next = NULL;
        return ret;
    } else {
        return NULL;
    }
}