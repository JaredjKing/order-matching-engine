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
    SkipListNode* head;
    SkipListNode* tail;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0.0, 1.0};

    int randomLevel();
    void findPredecessorsAndSuccessors(double price, SkipListNode* pred[], SkipListNode* succ[]);

   public:
    SkipList();
    ~SkipList();
    bool isEmpty();
    void addOrder(Order* order);
    bool removeNode(double price);
    SkipListNode* search(double price);
    SkipListNode* getLowestNode();
    SkipListNode* getHighestNode();

    void printSkipList() const {
        SkipListNode* current = head->next[0];
        std::cout << "--------- Skip List Contents: --------- \n";
        while (current != nullptr && current != tail) {
            std::cout << "Price Level: " << current->price << "\n";
            std::queue<Order*> orders = current->orders;
            while (!orders.empty()) {
                Order order = *orders.front();
                orders.pop();
                std::cout << "  Order ID: " << order.id
                          << " | Quantity Remaining: " << order.quantity_remaining
                          << " | Fulfillers: ";
                std::cout << "\n";
            }
            current = current->next[0];
        }
        std::cout << "--------- End of Skip List ---------\n";
    }
};
