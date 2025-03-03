#include <iostream>

#include "SkipList.hpp"

int main() {
    SkipList orderBook;

    orderBook.insert(1, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));
    orderBook.insert(100, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));
    orderBook.insert(50, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));
    orderBook.insert(75, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));
    orderBook.insert(50, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));
    orderBook.insert(11, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));

    SkipListNode* result = orderBook.search(11);
    std::cout << "result: " << result->price << "\n";

    orderBook.insert(51, Order(1, 100.5, 10, OrderType::LIMIT, OrderSide::BUY));

    SkipListNode* result1 = orderBook.search(51);
    std::cout << "result: " << result1->price << "\n";

    orderBook.remove(51);
    orderBook.remove(1);

    SkipListNode* result2 = orderBook.search(51);
    std::cout << "result: " << result2 << "\n";

    return 0;
}
