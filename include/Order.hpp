#pragma once

#include <cstdint>

#include "utils.h"

enum class OrderType { MARKET, LIMIT, STOP, STOP_LIMIT };
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

    inline bool isMarket() const { return type == OrderType::MARKET; }
    inline bool isLimit() const { return type == OrderType::LIMIT; }
    inline bool isStop() const { return type == OrderType::STOP; }
    inline bool isStopLimit() const { return type == OrderType::STOP_LIMIT; }
};