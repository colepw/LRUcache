#include <LRUCache.h>
#include <utility>
#include <unordered_map>
#include <memory>
#include <cstddef>
#include <optional>

LRUCache::LRUCache(std::size_t capacity) : capacity_(capacity) {
    head_.prev = nullptr;
    head_.next = &tail_;
    head_.key = 0;
    head_.value = 0;
    tail_.prev = &head_;
    tail_.next = nullptr;
    tail_.key = 0;
    tail_.value = 0;
    if (capacity_) store_.reserve(capacity_);
}

std::optional<int> LRUCache::get(int key) {
    auto it = store_.find(key); // See if key exists

    if (it == store_.end()) return std::nullopt; // Key DNE

    Node* n = it->second.get(); // Get and move raw ptr, return its value
    move_to_front(n);

    return n->value;
}

void LRUCache::put(int key, int value) {

}

void LRUCache::detach(Node* n) {
    if (!n || n == &head_ || n == &tail_ || !n->next || !n->prev) return;

    n->prev->next = n->next;
    n->next->prev = n->prev;

    n->prev = nullptr;
    n->next = nullptr;
}

void LRUCache::attach_front(Node* n) { // Detach n if needed, then reinsert at start of list
    detach(n);

    n->next = head_.next;
    n->prev = &head_;
    head_.next->prev = n;
    head_.next = n;
}