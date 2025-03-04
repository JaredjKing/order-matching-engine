#pragma once

#include <unordered_map>
#include <vector>

#include "SkipList.hpp"

class OrderBook {
   private:
    SkipList buy_orders;
    SkipList sell_orders;
    std::unordered_map<int, std::vector<int>> completed_orders;
    std::unordered_map<int, Order> submitted_orders;

   public:
    OrderBook();
    void addLimitOrder(Order& order);
    bool completeOrder(Order& order);
    bool matchMarketOrder(Order& order);
    bool matchLimitBuyOrder(Order& buyOrder);
    bool matchLimitSellOrder(Order& sellOrder);
    // void activateStopOrder(Order& order);
    // void activateStopLimitOrder(Order& order);
};