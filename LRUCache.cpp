#include "LRUCache.h"
#include <utility>
#include <unordered_map>
#include <memory>
#include <cstddef>
#include <optional>
#include <cassert>


// PUBLIC API FUNCTIONS


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

    check_invariants();

    return n->value;
}

void LRUCache::put(int key, int value) {


    check_invariants();
}

std::size_t LRUCache::size() const {

}

bool LRUCache::contains(int key) const {

}

bool LRUCache::erase(int key) {


    check_invariants();
}

void LRUCache::clear() {

}


// PRIVATE HELPERS


void LRUCache::detach(Node* n) {
    assert(n && n != &head_ && n != &tail_ && n->next && n->prev);

    n->prev->next = n->next;
    n->next->prev = n->prev;

    n->prev = nullptr;
    n->next = nullptr;
}

void LRUCache::attach_front(Node* n) {
    assert(n && n != &head_ && n != &tail_ && !n->next && !n->prev);

    n->next = head_.next;
    n->prev = &head_;
    n->next->prev = n;

    head_.next = n;
}

void LRUCache::move_to_front(Node* n) { // Detach n, then reinsert at start of list
    detach(n);
    attach_front(n);
}

void LRUCache::evict_lru() {
    assert(head_.next != &tail_);

    Node* v = tail_.prev;
    detach(v);

    store_.erase(v->key);

    check_invariants();
}

void LRUCache::check_invariants() const {

}