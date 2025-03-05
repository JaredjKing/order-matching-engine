#pragma once

#include <unordered_map>
#include <vector>

#include "SkipList.hpp"

class OrderBook {
   private:
    SkipList buy_orders;
    SkipList sell_orders;
    std::unordered_map<int, Order> submitted_orders;
    void fulfillOrders(Order& orderA, Order& orderB, int quantity);

   public:
    OrderBook();
    void addLimitOrder(Order&& order);
    bool completeOrder(Order& order);
    bool matchMarketOrder(Order& order);
    bool matchLimitBuyOrder(Order& buyOrder);
    bool matchLimitSellOrder(Order& sellOrder);
    // void activateStopOrder(Order& order);
    // void activateStopLimitOrder(Order& order);

    void printSellList() const { sell_orders.printSkipList(); }
    void printBuyList() const { buy_orders.printSkipList(); }
};