#pragma once

#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <random>

#include "SkipListNode.hpp"

static constexpr int MAX_LEVEL = 16;

class SkipList {
   private:
    std::shared_ptr<SkipListNode> head;
    std::shared_ptr<SkipListNode> tail;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0.0, 1.0};

    int randomLevel();
    void findPredecessorsAndSuccessors(double price, std::shared_ptr<SkipListNode> pred[],
                                       std::shared_ptr<SkipListNode> succ[]);

   public:
    SkipList();
    bool isEmpty();
    void addOrder(const Order&& order);
    bool removeNode(double price);
    std::shared_ptr<SkipListNode> search(double price);
    std::shared_ptr<SkipListNode> getLowestNode();
    std::shared_ptr<SkipListNode> getHighestNode();
    Order getNextOrder(double price);

    void printSkipList() const {
        std::shared_ptr<SkipListNode> current = head->next[0];
        std::cout << "--------- Skip List Contents: --------- \n";
        while (current != nullptr && current != tail) {
            std::cout << "Price Level: " << current->price << "\n";
            std::queue<Order> orders = current->orders;
            while (!orders.empty()) {
                Order order = orders.front();
                orders.pop();
                std::cout << "  Order ID: " << order.id
                          << " | Quantity Remaining: " << order.quantity_remaining
                          << " | Fulfillers: ";
                for (const auto& fulfiller : order.fulfillers) {
                    std::cout << "[Order ID: " << fulfiller[0] << ", Qty: " << fulfiller[1] << "] ";
                }
                std::cout << "\n";
            }
            current = current->next[0];
        }
        std::cout << "--------- End of Skip List ---------\n";
    }
};
