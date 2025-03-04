#include "SkipList.hpp"

#include <iostream>
#include <optional>

SkipList::SkipList() { head = (new SkipListNode(-1)); }

int SkipList::randomLevel() {
    int level = 1;
    while (distribution(rng) < 0.5 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

void SkipList::findPredecessorsAndSuccessors(double price, SkipListNode* pred[],
                                             SkipListNode* succ[]) {
    SkipListNode* curr = head;

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        SkipListNode* nextNode = curr->next[level];
        while (nextNode && nextNode->price < price) {
            curr = nextNode;
            nextNode = curr->next[level];
        }
        pred[level] = curr;
        succ[level] = nextNode;
    }
}

void SkipList::addOrder(const Order& order) {
    SkipListNode* pred[MAX_LEVEL];
    SkipListNode* succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(order.price, pred, succ);

    // If price level already exists
    if (succ[0] && succ[0]->price == order.price) {
        succ[0]->orders.push(order);
        return;
    }

    // Price level does not exist, need to create node
    int newLevel = randomLevel();
    SkipListNode* newNode = new SkipListNode(order.price);
    newNode->orders.push(order);

    // Insert newNode between pred and succ at each level the node exists
    for (int i = 0; i < newLevel; i++) {
        newNode->next[i] = succ[i];
        pred[i]->next[i] = newNode;
    }
}

bool SkipList::removeNode(double price) {
    SkipListNode* pred[MAX_LEVEL];
    SkipListNode* succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(price, pred, succ);

    if (!succ[0] || succ[0]->price != price) return false;  // Price not found

    // Unlink node from list
    for (int i = 0; i < MAX_LEVEL && succ[i] != nullptr && pred[i]->next[i] == succ[i]; i++) {
        pred[i]->next[i] = succ[i]->next[i];
    }

    delete succ[0];  // Free memory after removal
    return true;
}

SkipListNode* SkipList::search(double price) {
    SkipListNode* curr = head;

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (curr->next[level] && curr->next[level]->price < price) {
            curr = curr->next[level];
        }
    }
    curr = curr->next[0];

    return (curr && curr->price == price) ? curr : nullptr;
}

std::optional<Order> SkipList::getNextOrder(double price) {
    SkipListNode* node = search(price);
    if (node && !node->orders.empty()) {
        Order nextOrder = node->orders.front();
        node->orders.pop();
        return nextOrder;
    }
    return std::nullopt;
}

SkipListNode SkipList::getLowestNode() {}