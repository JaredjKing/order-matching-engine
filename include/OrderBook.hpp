#pragma once

#include <unordered_map>
#include <vector>

#include "SkipList.hpp"

class OrderBook {
   private:
    SkipList buy_orders;
    SkipList sell_orders;
    std::unordered_map<int, std::unique_ptr<Order>> submitted_orders;
    void fulfillOrders(Order* orderA, Order* orderB, int quantity);

   public:
    OrderBook();
    ~OrderBook();
    void addLimitOrder(Order&& order);
    // bool completeOrder(Order& order);
    // bool matchMarketOrder(Order& order);
    bool matchLimitBuyOrder(Order* buyOrder);
    bool matchLimitSellOrder(Order* sellOrder);
    // void activateStopOrder(Order& order);
    // void activateStopLimitOrder(Order& order);
    OrderBook(const OrderBook&) = delete;             // Prevent copying
    OrderBook& operator=(const OrderBook&) = delete;  // Prevent assignment

    void printSellList() const { sell_orders.printSkipList(); }
    void printBuyList() const { buy_orders.printSkipList(); }

    void printSubmittedOrders() const {
        std::cout << "--------- Submitted Orders ---------\n";

        if (submitted_orders.empty()) {
            std::cout << "No orders have been submitted.\n";
            return;
        }

        for (const auto& [id, order] : submitted_orders) {
            std::cout << "Order ID: " << order->id << " | Price: " << order->price
                      << " | Quantity Requested: " << order->quantity_requested
                      << " | Quantity Remaining: " << order->quantity_remaining << " | Type: "
                      << (order->isMarket()      ? "Market"
                          : order->isLimit()     ? "Limit"
                          : order->isStop()      ? "Stop"
                          : order->isStopLimit() ? "Stop-Limit"
                                                 : "Unknown")
                      << " | Side: " << (order->side == OrderSide::BUY ? "Buy" : "Sell");

            if (order->quantity_remaining == 0) {
                std::cout << " | **FILLED**";
            }

            if (!order->fulfillers.empty()) {
                std::cout << "  Fulfilled by: [";
                for (const auto& fulfiller : order->fulfillers) {
                    std::cout << "[Order ID: " << fulfiller[0] << ", Qty: " << fulfiller[1] << "]";
                }
                std::cout << "]\n";
            } else {
                std::cout << "\n";
            }
        }

        std::cout << "--------- End of Submitted Orders ---------\n";
    }
};