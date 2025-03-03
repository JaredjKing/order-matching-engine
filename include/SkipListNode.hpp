#pragma once

#include <atomic>
#include <queue>
#include <random>

#include "Order.hpp"

constexpr int MAX_LEVEL = 16;

struct SkipListNode {
    double price;
    std::queue<Order> orders;
    std::atomic<SkipListNode*> next[MAX_LEVEL];

    SkipListNode(double p) : price(p) {
        for (int i = 0; i < MAX_LEVEL; i++) {
            next[i].store(nullptr);
        }
    }
};