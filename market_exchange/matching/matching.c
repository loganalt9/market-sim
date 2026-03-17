#include "infra.h"
#include "matching.h"

int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

int fill_price_level(price_level* pl, int qty) {
    int filled = 0;

    while (pl->head && qty > 0) {
        order* curr = pl->head;
        int trade = min(curr->size, qty);

        curr->size -= trade;
        qty -= trade;
        filled += trade;

        if (curr->size == 0) {
            remove_order_price_level(pl);
        }
    }
    return filled;
}

execution_result submit_market_order(orderbook* ob, int qty, int is_ask) {
    heap* target_book;
    if (is_ask) {
        target_book = ob->bids;
    } else {
        target_book = ob->asks;
    }
    
    execution_result res;
    res.filled_qty = 0;
    res.total_cost = 0;

    while (qty > 0) {
        price_level* best_bid = heap_peek(target_book);

        if (!best_bid) {
            break;
        }

        int filled = fill_price_level(best_bid, qty);
        qty -= filled;
        res.filled_qty += filled;
        res.total_cost += filled * best_bid->price;

        if (!best_bid->head) {
            heap_pop(target_book);
        }
    }

    return res;
}

execution_result submit_limit_order(orderbook* ob, double price, int qty, int is_ask) {
    heap* target_book;
    heap* off_book; // for inserting after
    if (is_ask) {
        target_book = ob->bids;
        off_book = ob->asks;
    } else {
        target_book = ob->asks;
        off_book = ob->bids;
    }

    execution_result res;
    res.filled_qty = 0;
    res.total_cost = 0;

    while (qty > 0) {
        price_level* best_bid = heap_peek(target_book);

        if (!best_bid) {
            break;
        }

        int crosses;
        if (is_ask) {  // SELL
            crosses = (best_bid->price >= price);
        } else {       // BUY
            crosses = (best_bid->price <= price);
        }

        if (!crosses) break;

        int filled = fill_price_level(best_bid, qty);
        qty -= filled;
        res.filled_qty += filled;
        res.total_cost += filled * best_bid->price;

        if (!best_bid->head) {
            heap_pop(target_book);
        }
    }

    if (qty > 0) {  
        price_level* existing = 0;

        for (int i = 0; i < off_book->size; i++) {
            if (off_book->data[i]->price == price) {
                existing = off_book->data[i];
                break;
            }
        }

        if (existing) {
            order* new_order = create_order(1 /*FIX*/, qty);
            add_order_price_level(existing, new_order);
        } else {
            order* new_order = create_order(1 /*FIX*/, qty);
            price_level* new_pl = create_price_level(price);
            add_order_price_level(new_pl, new_order);
            heap_insert(off_book, new_pl);
        }
    }
    return res;
}

double get_best_bid(orderbook* ob) {
    price_level* pl = heap_peek(ob->bids);
    return pl ? pl->price : 0;
}

double get_best_ask(orderbook* ob) {
    price_level* pl = heap_peek(ob->asks);
    return pl ? pl->price : 0;
}