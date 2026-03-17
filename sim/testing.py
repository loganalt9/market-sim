import ctypes

if __name__ == "__main__":
    lib = ctypes.CDLL("./build/liblob.so")

    class ExecutionResult(ctypes.Structure):
        _fields_ = [
            ("filled_qty", ctypes.c_int),
            ("total_cost", ctypes.c_double)
        ]

    OrderBookPtr = ctypes.c_void_p

    lib.create_orderbook.argtypes = [ctypes.c_int]
    lib.create_orderbook.restype = OrderBookPtr

    lib.submit_market_order.argtypes = [OrderBookPtr, ctypes.c_int, ctypes.c_int]
    lib.submit_market_order.restype = ExecutionResult

    lib.submit_limit_order.argtypes = [OrderBookPtr, ctypes.c_double, ctypes.c_int, ctypes.c_int]
    lib.submit_limit_order.restype = ExecutionResult

    ob = lib.create_orderbook(100)

    lib.submit_limit_order(ob, 100.0, 10, 1)  # ask
    lib.submit_limit_order(ob, 99.0, 10, 1)   # ask

    result = lib.submit_market_order(ob, 5, 0)  # buy

    print(result.filled_qty)
    print(result.total_cost)
