#include <iostream>

#include "OrderBook.hpp"

int main() {
    OrderBook order_book;

    std::cout << "Entering Sell orders\n";
    order_book.addLimitOrder(Order(0, 100, 100, OrderType::LIMIT, OrderSide::SELL));
    order_book.addLimitOrder(Order(1, 100, 200, OrderType::LIMIT, OrderSide::SELL));
    order_book.addLimitOrder(Order(2, 200, 100, OrderType::LIMIT, OrderSide::SELL));
    order_book.addLimitOrder(Order(3, 200, 200, OrderType::LIMIT, OrderSide::SELL));
    order_book.addLimitOrder(Order(4, 300, 100, OrderType::LIMIT, OrderSide::SELL));
    order_book.addLimitOrder(Order(5, 300, 200, OrderType::LIMIT, OrderSide::SELL));
    order_book.printSellList();
    std::cout << "Orders entered\n";

    order_book.addLimitOrder(Order(6, 100, 150, OrderType::LIMIT, OrderSide::BUY));
    order_book.printSellList();
    order_book.addLimitOrder(Order(6, 100, 50, OrderType::LIMIT, OrderSide::BUY));
    order_book.printSellList();

    return 0;
}
