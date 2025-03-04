#include "OrderBook.hpp"

#include <iostream>

OrderBook::OrderBook() {
    buy_orders = SkipList();
    sell_orders = SkipList();
}

void OrderBook::addLimitOrder(Order& order) {
    switch (order.side) {
        case (OrderSide::BUY):
            if (!matchLimitBuyOrder(order)) {
                buy_orders.addOrder(order);
            }
            std::cout << "Unable to add order... \n";
        case (OrderSide::SELL):
            if (!matchLimitSellOrder(order)) {
                sell_orders.addOrder(order);
            }
            std::cout << "Unable to add order... \n";
    }
}

bool OrderBook::matchLimitBuyOrder(Order& buy_order) {
    if (sell_orders.empty) {
        return false;
    }

    SkipListNode lowest_priced_node = sell_orders.getLowestNode();
    if (lowest_priced_node.price <= buy_order.price) {
        while (buy_order.quantity > 0) {
            Order executing_order = lowest_priced_node.orders.front();
            if (executing_order.quantity > buy_order.quantity) {
                executing_order.quantity -= buy_order.quantity;

                // buy_order is complete..
                buy_order.quantity = 0;

                // need to log completed order...

                return true;

            } else if (executing_order.quantity < buy_order.quantity) {
                buy_order.quantity -= executing_order.quantity;

                // executing_order is complete
                executing_order.quantity = 0;
                lowest_priced_node.orders.pop();

                // Get next executing order
                if (lowest_priced_node.orders.size() == 0) {
                    sell_orders.removeNode(lowest_priced_node.price);
                    lowest_priced_node = sell_orders.getLowestNode();
                    if (lowest_priced_node.price > buy_order.price) {
                        return false;
                    }
                }

                // need to log completed order...

            } else {
                // perfect match, both orders are complete
                buy_order.quantity == 0;
                executing_order.quantity == 0;
                lowest_priced_node.orders.pop();

                // remove executing order
                if (lowest_priced_node.orders.size() == 0) {
                    sell_orders.removeNode(lowest_priced_node.price);
                }

                return true;

                // need to log completed order...
            }
        }
    }
}