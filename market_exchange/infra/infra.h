#ifndef HEAP_H
#define HEAP_H

typedef struct order {
    int id;
    int size;
    struct order* next;
} order;

typedef struct price_level {
    double price;
    struct order* head;
    struct order* tail;
} price_level;

typedef int (*price_cmp)(double a, double b); // 0: min, 1: max (heap type)

typedef struct heap {
    struct price_level** data;
    int size;
    int capacity;
    price_cmp cmp;
} heap;

//orders
order* create_order(int id, int size);
void delete_order(price_level* level, int id);

//price levels
price_level* create_price_level(double price);
void delete_price_level(price_level* pl);
void add_order_price_level(price_level* pl, order* o);

//heaps
heap* create_heap(int cap, int is_max);
void delete_heap(heap* h);
price_level* heap_pop(heap* h);
void heap_insert(heap* h, price_level* p);

#endif