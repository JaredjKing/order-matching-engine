#include "SkipList.hpp"

#include <iostream>

SkipList::SkipList() {
    head = std::make_shared<SkipListNode>(-1);
    tail = std::make_shared<SkipListNode>(-1);
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

void SkipList::findPredecessorsAndSuccessors(double price, std::shared_ptr<SkipListNode> pred[],
                                             std::shared_ptr<SkipListNode> succ[]) {
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

void SkipList::addOrder(const Order&& order) {
    std::shared_ptr<SkipListNode> pred[MAX_LEVEL];
    std::shared_ptr<SkipListNode> succ[MAX_LEVEL];

    findPredecessorsAndSuccessors(order.price, pred, succ);

    // If price level already exists
    if (succ[0] && succ[0]->price == order.price) {
        succ[0]->orders.push(std::move(order));
        return;
    }

    // Price level does not exist, need to create node
    int newLevel = randomLevel();
    auto newNode = std::make_shared<SkipListNode>(order.price);
    newNode->orders.push(std::move(order));

    // Insert newNode between pred and succ at each level the node exists
    for (int i = 0; i < newLevel; i++) {
        newNode->next[i] = succ[i];
        newNode->prev[i] = pred[i];
        pred[i]->next[i] = newNode;
        succ[i]->prev[i] = newNode;
    }
}

bool SkipList::removeNode(double price) {
    std::shared_ptr<SkipListNode> pred[MAX_LEVEL] = {nullptr};
    std::shared_ptr<SkipListNode> succ[MAX_LEVEL] = {nullptr};

    findPredecessorsAndSuccessors(price, pred, succ);

    if (!succ[0] || succ[0]->price != price) return false;  // Price not found

    // Store the node to be deleted
    std::shared_ptr<SkipListNode> target = succ[0];

    // Unlink node from list at each level
    for (int i = 0; i < MAX_LEVEL; i++) {
        if (pred[i] && pred[i]->next[i] == target) {
            pred[i]->next[i] = target->next[i];
            if (target->next[i]) {
                target->next[i]->prev[i] = pred[i];
            }
        }
    }

    return true;
}

std::shared_ptr<SkipListNode> SkipList::search(double price) {
    auto curr = head;

    for (int level = MAX_LEVEL - 1; level >= 0; level--) {
        while (curr->next[level] != tail && curr->next[level]->price < price) {
            curr = curr->next[level];
        }
    }
    curr = curr->next[0];

    return (curr != tail && curr->price == price) ? curr : nullptr;
}

Order SkipList::getNextOrder(double price) {
    auto node = search(price);
    if (node && !node->orders.empty()) {
        Order nextOrder = node->orders.front();
        node->orders.pop();
        return nextOrder;
    }
    throw std::runtime_error("No order found at the given price");
}

std::shared_ptr<SkipListNode> SkipList::getLowestNode() {
    return (head->next[0] != tail) ? head->next[0] : nullptr;
}

std::shared_ptr<SkipListNode> SkipList::getHighestNode() {
    return (tail->prev[0] != head) ? tail->prev[0] : nullptr;
}

bool SkipList::isEmpty() {
    if (!head) {
        return true;
    }
    return false;
}