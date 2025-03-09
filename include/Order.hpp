#pragma once

#include <cstdint>

#include "utils.h"

enum class OrderType : uint8_t { MARKET, LIMIT, STOP, STOP_LIMIT };
enum class OrderSide : uint8_t { BUY, SELL };

struct Order {
    uint64_t timestamp;           // 8B
    double price;                 // 8B
    uint32_t quantity_requested;  // 4B
    uint32_t quantity_remaining;  // 4B
    int32_t id;                   // 4B
    OrderType type;               // 1B
    OrderSide side;               // 1B
    uint16_t _padding;            // 2B (aligns memory to 32B)

    explicit Order(int id, double price, uint32_t quantity_requested, OrderType type,
                   OrderSide side, uint64_t timestamp = getCurrentTimestamp())
        : timestamp(timestamp),
          price(price),
          quantity_requested(quantity_requested),
          quantity_remaining(quantity_requested),
          id(id),
          type(type),
          side(side) {}

    inline bool isMarket() const { return type == OrderType::MARKET; }
    inline bool isLimit() const { return type == OrderType::LIMIT; }
    inline bool isStop() const { return type == OrderType::STOP; }
    inline bool isStopLimit() const { return type == OrderType::STOP_LIMIT; }
};