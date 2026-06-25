typedef struct execution_result {
    int filled_qty;
    double total_cost;
} execution_result;

execution_result submit_market_order(orderbook* ob, int qty, int is_ask);
execution_result submit_limit_order(orderbook* ob, double price, int qty, int is_ask);
double get_best_bid(orderbook* ob);
double get_best_ask(orderbook* ob);
int get_best_bid_size(orderbook* ob);
int get_best_ask_size(orderbook* ob);