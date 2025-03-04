#pragma once

#include <array>
#include <optional>
#include <queue>
#include <random>

#include "SkipListNode.hpp"

static constexpr int MAX_LEVEL = 16;

class SkipList {
   private:
    SkipListNode* head;
    std::mt19937 rng{std::random_device{}()};
    std::uniform_real_distribution<double> distribution{0.0, 1.0};

    int randomLevel();
    void findPredecessorsAndSuccessors(double price, SkipListNode* pred[], SkipListNode* succ[]);

   public:
    bool empty;
    SkipList();
    void addOrder(const Order& order);
    bool removeNode(double price);
    std::optional<Order> getNextOrder(double price);
    SkipListNode* search(double price);
    SkipListNode getLowestNode();
};
