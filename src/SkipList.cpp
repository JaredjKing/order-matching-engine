#include "SkipList.hpp"

#include <iostream>

SkipList::SkipList() {
    head = new SkipListNode(-1);
    tail = new SkipListNode(-1);
    for (int i = 0; i < MAX_LEVEL; i++) {
        head->next[i] = tail;
        tail->prev[i] = head;
    }
}

int SkipList::randomLevel() {
    int level = 1;
    while (distribution(rng) < 0.5 && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

SkipList::~SkipList() {
    SkipListNode* current = head->next[0];

    while (current != tail) {
        SkipListNode* nextNode = current->next[0];
        delete current;
        current = nextNode;
    }
    delete tail;
    delete head;
}

void SkipList::findPredecessorsAndSuccessors(double price, SkipListNode* pred[],
                                             SkipListNode* succ[]) {
    auto curr = head;

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        auto nextNode = curr->next[level];
        while (nextNode != tail && nextNode->price < price) {
            curr = nextNode;
            nextNode = curr->next[level];
        }
        pred[level] = curr;
        succ[level] = nextNode;
    }
}

void SkipList::addOrder(Order* order) {
    SkipListNode* pred[MAX_LEVEL];
    SkipListNode* succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(order->price, pred, succ);

    // If price level already exists
    if (succ[0] && succ[0]->price == order->price) {
        succ[0]->orders.push(order);
        return;
    }

    // Price level does not exist, need to create node
    int newLevel = randomLevel();
    auto newNode = new SkipListNode(order->price);
    newNode->orders.push(order);

    // Insert newNode between pred and succ at each level the node exists
    for (int i = 0; i < newLevel; i++) {
        newNode->next[i] = succ[i];
        newNode->prev[i] = pred[i];
        pred[i]->next[i] = newNode;
        succ[i]->prev[i] = newNode;
    }
}

bool SkipList::removeNode(double price) {
    SkipListNode* pred[MAX_LEVEL] = {nullptr};
    SkipListNode* succ[MAX_LEVEL] = {nullptr};

    findPredecessorsAndSuccessors(price, pred, succ);

    if (!succ[0] || succ[0]->price != price) return false;  // Price not found

    // Store the node to be deleted
    SkipListNode* target = succ[0];

    // Unlink node from list at each level
    for (int i = 0; i < MAX_LEVEL; i++) {
        if (pred[i] && pred[i]->next[i] == target) {
            pred[i]->next[i] = target->next[i];
            if (target->next[i]) {
                target->next[i]->prev[i] = pred[i];
            }
        }
    }

    delete target;
    return true;
}

SkipListNode* SkipList::search(double price) {
    auto curr = head;

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (curr->next[level] != tail && curr->next[level]->price < price) {
            curr = curr->next[level];
        }
    }
    curr = curr->next[0];

    return (curr != tail && curr->price == price) ? curr : nullptr;
}

SkipListNode* SkipList::getLowestNode() {
    return (head->next[0] != tail) ? head->next[0] : nullptr;
}

SkipListNode* SkipList::getHighestNode() {
    return (tail->prev[0] != head) ? tail->prev[0] : nullptr;
}

bool SkipList::isEmpty() {
    if (!head) {
        return true;
    }
    return false;
}