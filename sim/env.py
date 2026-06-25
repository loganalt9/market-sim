import numpy as np
from lob_wrapper import *

class LOBEnv:
    def __init__(self):
        self.max_steps = 50
        self.ob = None
        self.step_count = 0
        self.inventory = 0
        self.prev_mid = 0.0

    def reset(self):
        import random
        self.ob = lib.create_orderbook(100)
        self.step_count = 0
        self.inventory = 0
        for _ in range(10):
            bid_price = 100.0 - random.uniform(0.05, 1.0)
            ask_price = 100.0 + random.uniform(0.05, 1.0)
            lib.submit_limit_order(self.ob, bid_price, random.randint(5, 20), 0)
            lib.submit_limit_order(self.ob, ask_price, random.randint(5, 20), 1)
        bid = lib.get_best_bid(self.ob)
        ask = lib.get_best_ask(self.ob)
        self.prev_mid = (bid + ask) / 2.0
        return self._get_state()

    def step(self, action):
        """
        0: hold
        1: market buy  (+10 inventory)
        2: market sell (-10 inventory)
        """
        self._random_order_flow()

        bid = lib.get_best_bid(self.ob)
        ask = lib.get_best_ask(self.ob)
        mid = (bid + ask) / 2.0

        slippage_reward = 0.0

        if action == 1:  # BUY
            res = lib.submit_market_order(self.ob, 10, 0)
            if res.filled_qty > 0:
                actual = res.total_cost / res.filled_qty
                slippage_reward = -(actual - ask)
                self.inventory += res.filled_qty

        elif action == 2:  # SELL
            res = lib.submit_market_order(self.ob, 10, 1)
            if res.filled_qty > 0:
                actual = res.total_cost / res.filled_qty
                slippage_reward = -(bid - actual)
                self.inventory -= res.filled_qty

        mtm_reward = self.inventory * (mid - self.prev_mid)
        reward = slippage_reward + mtm_reward

        self.prev_mid = mid
        self.step_count += 1
        done = self.step_count >= self.max_steps
        return self._get_state(), reward, done, {}

    def _random_order_flow(self):
        import random
        size = random.randint(1, 10)
        if random.random() < 0.5:
            lib.submit_limit_order(self.ob, 100.0 - random.uniform(0.05, 1.0), size, 0)
        else:
            lib.submit_limit_order(self.ob, 100.0 + random.uniform(0.05, 1.0), size, 1)

    def _get_state(self):
        bid = lib.get_best_bid(self.ob)
        ask = lib.get_best_ask(self.ob)
        mid = (bid + ask) / 2.0
        spread = ask - bid
        bid_size = lib.get_best_bid_size(self.ob)
        ask_size = lib.get_best_ask_size(self.ob)
        steps_remaining = (self.max_steps - self.step_count) / self.max_steps

        return np.array([
            mid - 100.0,       # centered near 0
            spread,
            float(bid_size),
            float(ask_size),
            float(self.inventory),
            steps_remaining,
        ], dtype=np.float32)
