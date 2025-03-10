#pragma once

#include <unordered_map>
#include <vector>

#include "OrderMemoryPool.hpp"
#include "SkipList.hpp"

class OrderBook {
   private:
    SkipList buy_orders;
    SkipList sell_orders;
    OrderMemoryPool orderPool;
    // std::unordered_map<int, Order*> submitted_orders;
    std::unordered_map<int, std::vector<std::array<double, 3>>> order_completions;
    void fulfillOrders(Order* orderA, Order* orderB, int quantity);

   public:
    OrderBook();
    void addLimitOrder(int id, double price, uint32_t quantity, OrderType type, OrderSide side);
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
};