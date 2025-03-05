#pragma once

#include <cstdint>

#include "utils.h"

enum class OrderType { MARKET, LIMIT, STOP, STOP_LIMIT };
enum class OrderSide { BUY, SELL };

struct Order {
    int id;
    double price;
    uint32_t quantity_requested;
    uint32_t quantity_remaining;
    OrderType type;
    OrderSide side;
    uint64_t timestamp;                          // Nano seconds since epoch
    std::vector<std::array<int, 2>> fulfillers;  // order IDs of order(s) that fufilled the order

    Order(uint64_t id, double price, uint32_t quantity_requested, OrderType type, OrderSide side,
          uint64_t timestamp = getCurrentTimestamp(),
          std::vector<std::array<int, 2>> fulfillers = {})
        : id(id),
          price(price),
          quantity_requested(quantity_requested),
          quantity_remaining(quantity_requested),
          type(type),
          side(side),
          timestamp(timestamp),
          fulfillers(std::move(fulfillers)) {};

    inline bool isMarket() const { return type == OrderType::MARKET; }
    inline bool isLimit() const { return type == OrderType::LIMIT; }
    inline bool isStop() const { return type == OrderType::STOP; }
    inline bool isStopLimit() const { return type == OrderType::STOP_LIMIT; }
};