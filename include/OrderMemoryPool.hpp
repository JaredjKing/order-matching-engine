#pragma once

#include <array>
#include <cassert>
#include <stack>

#include "Order.hpp"

constexpr size_t BATCH_SIZE = 30000;

class OrderMemoryPool {
   private:
    std::array<Order, BATCH_SIZE> orders;
    std::stack<Order*> freeList;

   public:
    OrderMemoryPool() {
        for (size_t i = 0; i < BATCH_SIZE; ++i) {
            freeList.push(&orders[i]);
        }
    }

    Order* allocate(int id, double price, uint32_t quantity_requested, OrderType type,
                    OrderSide side) {
        assert(!freeList.empty() && "Memory pool exhausted!");

        Order* order = freeList.top();
        freeList.pop();

        return new (order) Order(id, price, quantity_requested, type, side);
    }

    void deallocate(Order* order) {
        if (!order) return;
        order->~Order();
        freeList.push(order);
    }
};
