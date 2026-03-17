#include "infra.h"
#include <stdlib.h>

orderbook* create_orderbook(int heap_cap) {
    orderbook* ob = malloc(sizeof(orderbook));
    ob->asks = create_heap(heap_cap,0);
    ob->bids = create_heap(heap_cap, 1);
    return ob;
}

void delete_orderbook(orderbook* ob) {
    delete_heap(ob->asks);
    delete_heap(ob->bids);
    free(ob);
}