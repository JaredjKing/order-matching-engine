#pragma once

#include <random>

#include "SkipListNode.hpp"

class LockFreeSkipList {
   private:
    std::atomic<SkipListNode*> head;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0.0, 1.0};

    int randomLevel();
    void findPredecessorsAndSuccessors(double price, SkipListNode* pred[], SkipListNode* succ[]);

   public:
    LockFreeSkipList();
    void insert(double price, const Order& order);
    bool remove(double price);
    Order getNextOrder(double price);
    SkipListNode* search(double price);
};
