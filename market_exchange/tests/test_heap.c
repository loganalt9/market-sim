#include "../infra/infra.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>

int test_max_add_pop() {
    srand(time(0));

    heap* bids = create_heap(5, 1);

    // create price levels
    price_level* pl80 = create_price_level(85.0);
    price_level* pl90 = create_price_level(90.0);
    price_level* pl95 = create_price_level(95.0);

    // create orders
    order** o = malloc(sizeof(order*)*5);

    for (int i = 0; i < 5; i++) {
        o[i] = create_order(i, ((rand()%9)+1)*10);
    }

    // add orders to price_levels

    add_order_price_level(pl80, o[0]);
    add_order_price_level(pl80, o[1]);
    add_order_price_level(pl90, o[2]);
    add_order_price_level(pl95, o[3]);
    add_order_price_level(pl95, o[4]);

    heap_insert(bids, pl80);
    heap_insert(bids, pl90);
    heap_insert(bids, pl95);

    assert(heap_pop(bids) == pl95);

    return 1;
}

int main(void) {
    assert(test_max_add_pop());
    return 1;
}