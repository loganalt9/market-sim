import ctypes
from ctypes import c_int, c_double, Structure, POINTER

lib = ctypes.CDLL("./build/liblob.so")

class ExecutionResult(Structure):
    _fields_ = [
        ("filled_qty", c_int),
        ("total_cost", c_double),
    ]

class OrderBook(ctypes.Structure):
    pass

lib.create_orderbook.argtypes = [c_int]
lib.create_orderbook.restype = POINTER(OrderBook)

lib.submit_market_order.argtypes = [POINTER(OrderBook), c_int, c_int]
lib.submit_market_order.restype = ExecutionResult

lib.submit_limit_order.argtypes = [POINTER(OrderBook), c_double, c_int, c_int]
lib.submit_limit_order.restype = ExecutionResult