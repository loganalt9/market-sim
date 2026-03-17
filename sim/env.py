import numpy as np
from lob_wrapper import *

class LOBEnv:
    def __init__(self):
        self.ob = lib.create_orderbook(100)
        self.max_steps = 50
        self.step_count = 0

    def reset(self):
        self.ob = lib.create_orderbook(100)
        self.step_count = 0
        return self._get_state()

    def step(self, action):
        """
        0: hold
        1: market buy
        2: market sell
        """
        bid = lib.get_best_bid(self.ob)
        ask = lib.get_best_ask(self.ob)

        reward = 0

        if action == 1:  # BUY
            expected = ask
            res = lib.submit_market_order(self.ob, 10, 0)

            if res.filled_qty > 0:
                actual = res.total_cost / res.filled_qty
                slippage = actual - expected
                reward = -slippage

        elif action == 2:  # SELL
            expected = bid
            res = lib.submit_market_order(self.ob, 10, 1)

            if res.filled_qty > 0:
                actual = res.total_cost / res.filled_qty
                slippage = expected - actual
                reward = -slippage

    def _random_order_flow(self):
        import random

        price = 100 + random.uniform(-1, 1)
        size = random.randint(1, 10)

        if random.random() < 0.5:
            lib.submit_limit_order(self.ob, price, size, 1)
        else:
            lib.submit_limit_order(self.ob, price, size, 0)

    def _get_state(self):
        # simple state for now
        return np.array([
            self.step_count
        ], dtype=np.float32)