#pragma once

#include <array>
#include <queue>

#include "Order.hpp"
struct SkipListNode {
    static constexpr int MAX_LEVEL = 128;
    double price;
    std::queue<Order*> orders;
    std::array<SkipListNode*, MAX_LEVEL> next;
    std::array<SkipListNode*, MAX_LEVEL> prev;

    SkipListNode(double price) : price(price) {
        next.fill(nullptr);  // Initialize all levels to nullptr
        prev.fill(nullptr);
    }
};