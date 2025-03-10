#pragma once

#include <array>
#include <queue>

#include "Order.hpp"

static constexpr int MAX_LEVEL = 128;

struct SkipListNode {
    double price;
    std::queue<Order*> orders;
    std::array<SkipListNode*, MAX_LEVEL> next;
    std::array<SkipListNode*, MAX_LEVEL> prev;

    SkipListNode(double price) : price(price) {
        next.fill(nullptr);
        prev.fill(nullptr);
    }
};