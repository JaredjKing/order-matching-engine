#include "OrderBook.hpp"

#include <iostream>

OrderBook::OrderBook() : buy_orders(), sell_orders() {}

void OrderBook::addLimitOrder(int id, double price, uint32_t quantity, OrderType type,
                              OrderSide side) {
    Order* order = orderPool.allocate(id, price, quantity, type, side);
    if (!order) {
        std::cerr << "Order pool exhausted! Consider resetting the batch.\n";
        return;
    }

    switch (order->side) {
        case OrderSide::BUY:
            if (!matchLimitBuyOrder(order)) {
                buy_orders.addOrder(order);
            }
            break;
        case OrderSide::SELL:
            if (!matchLimitSellOrder(order)) {
                sell_orders.addOrder(order);
            }
            break;
    }
}

bool OrderBook::matchLimitBuyOrder(Order* buy_order) {
    if (sell_orders.isEmpty()) {
        return false;
    }

    SkipListNode* lowest_priced_node = sell_orders.getLowestNode();
    if (!lowest_priced_node) {
        return false;
    }

    // Check if the lowest-priced order satisfies the buy order
    if (lowest_priced_node->price > buy_order->price || lowest_priced_node->orders.empty()) {
        return false;
    }

    while (buy_order->quantity_remaining > 0) {
        Order* executing_order = lowest_priced_node->orders.front();

        int buy_amount = buy_order->quantity_remaining;
        int executing_amount = executing_order->quantity_remaining;

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
            if (lowest_priced_node->price > buy_order->price) {
                return false;
            }
        }
    }

    return true;
}

void OrderBook::fulfillOrders(Order* orderA, Order* orderB, int quantity) {
    orderA->quantity_remaining -= quantity;
    orderB->quantity_remaining -= quantity;
    order_completions[orderA->id].push_back(
        std::array<double, 3>{(double)orderB->id, (double)quantity, orderA->price});
    order_completions[orderA->id].push_back(
        std::array<double, 3>{(double)orderB->id, (double)quantity, orderA->price});
    return;
}

bool OrderBook::matchLimitSellOrder(Order* sell_order) {
    if (buy_orders.isEmpty()) {
        return false;
    }

    SkipListNode* highest_priced_node = buy_orders.getHighestNode();
    if (!highest_priced_node) {
        return false;
    }

    // Check if the highest-priced buy order satisfies the sell order
    if (highest_priced_node->price < sell_order->price || highest_priced_node->orders.empty()) {
        return false;
    }

    while (sell_order->quantity_remaining > 0) {
        Order* executing_order = highest_priced_node->orders.front();

        int sell_amount = sell_order->quantity_remaining;
        int executing_amount = executing_order->quantity_remaining;

        // Execute order fulfillment
        fulfillOrders(executing_order, sell_order, std::min(sell_amount, executing_amount));

        if (executing_amount > sell_amount) {
            // Sell order is fully filled, but executing buy order still has remaining quantity
            return true;
        }

        // Remove the fully executed buy order
        highest_priced_node->orders.pop();

        // If the price level is empty, remove it and move to the next highest price level
        if (highest_priced_node->orders.empty()) {
            buy_orders.removeNode(highest_priced_node->price);

            if (buy_orders.isEmpty()) {
                return false;
            }

            highest_priced_node = buy_orders.getHighestNode();
            if (!highest_priced_node || highest_priced_node->price < sell_order->price) {
                return false;
            }
        }
    }

    return true;
}
