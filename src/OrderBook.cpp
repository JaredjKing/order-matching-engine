#include "OrderBook.hpp"

#include <iostream>

OrderBook::OrderBook() {
    buy_orders = SkipList();
    sell_orders = SkipList();
}

void OrderBook::addLimitOrder(Order&& order) {
    switch (order.side) {
        case (OrderSide::BUY):
            if (!matchLimitBuyOrder(order)) {
                buy_orders.addOrder(std::move(order));
            }
            break;
        case (OrderSide::SELL):
            // if (!matchLimitSellOrder(order)) {
            sell_orders.addOrder(std::move(order));
            // }
            break;
    }
}

bool OrderBook::matchLimitBuyOrder(Order& buy_order) {
    if (sell_orders.isEmpty()) {
        return false;
    }

    std::shared_ptr<SkipListNode> lowest_priced_node = sell_orders.getLowestNode();
    if (!lowest_priced_node) {
        return false;
    }

    // Check if the lowest-priced order satisfies the buy order
    if (lowest_priced_node->price > buy_order.price || lowest_priced_node->orders.empty()) {
        return false;
    }

    while (buy_order.quantity_remaining > 0) {
        Order& executing_order = lowest_priced_node->orders.front();

        int buy_amount = buy_order.quantity_remaining;
        int executing_amount = executing_order.quantity_remaining;

        // Execute order fulfillment
        fulfillOrders(executing_order, buy_order, std::min(buy_amount, executing_amount));

        if (executing_amount > buy_amount) {
            // Buy order is fully filled, but executing order still has remaining quantity
            return true;
        }

        // Remove the fully executed order
        lowest_priced_node->orders.pop();

        // If the price level is empty, remove it and move to the next lowest price
        if (lowest_priced_node->orders.empty()) {
            sell_orders.removeNode(lowest_priced_node->price);

            if (sell_orders.isEmpty()) {
                return false;  // No more sell orders left
            }

            lowest_priced_node = sell_orders.getLowestNode();
            if (!lowest_priced_node) {
                return false;
            }

            // If the new lowest price is above the buy order's limit, stop processing
            if (lowest_priced_node->price > buy_order.price) {
                return false;
            }
        }
    }

    return true;
}

void OrderBook::fulfillOrders(Order& orderA, Order& orderB, int quantity) {
    orderA.quantity_remaining -= quantity;
    orderB.quantity_remaining -= quantity;
    orderA.fulfillers.emplace_back(std::array<int, 2>{orderB.id, quantity});
    orderB.fulfillers.emplace_back(std::array<int, 2>{orderB.id, quantity});
    return;
}
