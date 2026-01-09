#include "../infra/infra.h" //TODO: fix this
#include <assert.h>
#include <stdio.h>

// Test create and delete an order/price_level
int test_order_price_level() {
    order* o1_1 = create_order(1, 10);
    order* o1_2 = create_order(2, 20);
    o1_1->next = o1_2;

    order* o2_1 = create_order(3, 50);

    assert (o1_1->id == 1 && o1_1->size == 10);
    assert (o1_2->id == 2 && o1_2->size == 20);
    assert (o2_1->id == 3 && o2_1->size == 50);

    price_level* p1 = create_price_level(50.0);
    price_level* p2 = create_price_level(65.0);

    p1->head = o1_1;
    p1->tail = o1_2;

    p2->head = o2_1;
    p2->tail = o2_1;

    delete_price_level(p1);
    delete_price_level(p2);

    return 1;
}

int main(void) {
    assert(test_order_price_level());
    return 1;
}