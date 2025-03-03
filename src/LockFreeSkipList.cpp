#include "LockFreeSkipList.hpp"

#include <iostream>

LockFreeSkipList::LockFreeSkipList() { head.store(new SkipListNode(-1)); }

int LockFreeSkipList::randomLevel() {
    int level = 1;
    while (distribution(rng) < 0.5 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

void LockFreeSkipList::findPredecessorsAndSuccessors(double price, SkipListNode* pred[],
                                                     SkipListNode* succ[]) {
    SkipListNode* curr = head.load();

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (curr->next[level].load() && curr->next[level].load()->price < price) {
            curr = curr->next[level].load();
        }
        pred[level] = curr;
        succ[level] = curr->next[level].load();
    }
}

void LockFreeSkipList::insert(double price, const Order& order) {
    SkipListNode* pred[MAX_LEVEL];
    SkipListNode* succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(price, pred, succ);

    // If price level already exists
    if (succ[0] && succ[0]->price == price) {
        succ[0]->orders.push(order);
        return;
    }

    // Price level does not exist, need to create node
    int newLevel = randomLevel();
    SkipListNode* newNode = new SkipListNode(price);
    newNode->orders.push(order);

    // Insert newNode between pred and succ at each level the node exists
    for (int i = 0; i < newLevel; i++) {
        SkipListNode* expected = pred[i]->next[i].load();
        do {
            newNode->next[i] = pred[i]->next[i].load();
        } while (!pred[i]->next[i].compare_exchange_weak(expected, newNode));
        // retry if another thread modified pred[i]->next[i]
    }
}

bool LockFreeSkipList::remove(double price) {
    SkipListNode* pred[MAX_LEVEL];
    SkipListNode* succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(price, pred, succ);

    if (!succ[0] || succ[0]->price != price) return false;

    // Remove node from between pred and success at each level node exists
    for (int i = 0; i < MAX_LEVEL && succ[i] != nullptr && pred[i]->next[i].load() == succ[i];
         i++) {
        pred[i]->next[i].compare_exchange_strong(succ[i], succ[i]->next[i].load());
    }

    delete succ[0];
    return true;
}

SkipListNode* LockFreeSkipList::search(double price) {
    SkipListNode* curr = head.load();

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (curr->next[level].load() && curr->next[level].load()->price < price) {
            curr = curr->next[level].load();
        }
    }
    curr = curr->next[0].load();

    return (curr && curr->price == price) ? curr  : nullptr;
}

Order LockFreeSkipList::getNextOrder(double price) {
    SkipListNode* node = search(price);
    if (node && !node->orders.empty()) {
        Order nextOrder = node->orders.front();
        node->orders.pop();
        return nextOrder;
    }
    throw std::runtime_error("No orders available at this price.");
}