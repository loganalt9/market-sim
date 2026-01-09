#include "infra.h"
#include <stdlib.h>

int min_cmp(double a, double b) { return a < b; }
int max_cmp(double a, double b) { return a > b; }


heap* create_heap(int cap, int is_max) {
    heap* h = malloc(sizeof(heap));
    h->size = 0;
    h->capacity = cap;
    h->data = malloc(sizeof(heap*)*cap);
    h->cmp = is_max ? max_cmp : min_cmp;

    return h;
}

void delete_heap(heap* h) {
    int cap = h->capacity;
    for (int i = 0; i < cap; i++) {
        price_level* pl = h->data[i];
        if (pl != NULL) {
            free(pl);
            pl = NULL;
        }
    }
    free(h);
}

void heap_swap(heap* h, int i, int j) {
    price_level* tmp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = tmp;
}

void heap_bubble_up(heap* h, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (!h->cmp(h->data[i]->price, h->data[parent]->price))
            break;
        heap_swap(h, i, parent);
        i = parent;
    }
}

void heap_heapify(heap* h, int i) {
    while (1) {
        int left = 2*i + 1;
        int right = 2*i + 2;
        int best = i;

        if (left < h->size && h->cmp(h->data[left]->price, h->data[best]->price))
            best = left;
        if (right < h->size && h->cmp(h->data[right]->price, h->data[best]->price))
            best = right;

        if (best == i) break;

        heap_swap(h, i, best);
        i = best;
    }
}

void heap_insert(heap* h, price_level* p) {
    h->data[h->size] = p;
    heap_bubble_up(h, h->size);
    h->size++;
}

price_level* heap_pop(heap* h) {
    if (h->size == 0) return NULL;

    price_level* best = h->data[0];
    h->size--;
    h->data[0] = h->data[h->size];
    heap_heapify(h, 0);

    return best;
}

price_level* heap_peak(heap* h) {
    if (h->size == 0) return NULL;

    return h->data[0];
}