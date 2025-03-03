#pragma once

#include <cstdint>

#include "utils.h"

enum class OrderType { MARKET, LIMIT };
enum class OrderSide { BUY, SELL };

struct Order {
    uint64_t id;
    double price;
    uint32_t quantity;
    OrderType type;
    OrderSide side;
    uint64_t timestamp;  // Nano seconds since epoch

    Order(uint64_t id, double price, uint32_t quantity, OrderType type, OrderSide side,
          uint64_t timestamp = getCurrentTimestamp())
        : id(id), price(price), quantity(quantity), type(type), side(side), timestamp(timestamp) {};
};