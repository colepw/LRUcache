#include "LRUCache.h"
#include <cassert>


// PUBLIC API FUNCTIONS


LRUCache::LRUCache(std::size_t capacity) : capacity_(capacity) {
    head_.prev = nullptr;
    head_.next = &tail_;
    
    tail_.prev = &head_;
    tail_.next = nullptr;
    
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
    if (!capacity_) return;

    auto it = store_.find(key);

    if (it != store_.end()) {
        Node* n = it->second.get();
        n->value = value;
        move_to_front(n);
    } else {
        std::unique_ptr<Node> n = std::make_unique<Node>();
        n->next = nullptr;
        n->prev = nullptr;
        n->key = key;
        n->value = value;

        auto [p, emplaced] = store_.try_emplace(key, std::move(n));

        assert(emplaced);

        attach_front(p->second.get());

        if (store_.size() > capacity_) evict_lru();
    }

    check_invariants();
}

std::size_t LRUCache::size() const noexcept {
    return store_.size();
}

bool LRUCache::empty() const noexcept {
    return !size();
}

bool LRUCache::contains(int key) const noexcept {
    auto it = store_.find(key);

    return (it != store_.end());
}

bool LRUCache::erase(int key) {
    auto it = store_.find(key);

    if (it == store_.end()) return false;

    Node* n = it->second.get();
    detach(n);

    store_.erase(it);

    check_invariants();

    return true;
}

void LRUCache::clear() noexcept {
    store_.clear();

    head_.next = &tail_;
    tail_.prev = &head_;

    check_invariants();
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
}

void LRUCache::check_invariants() const {
#ifndef NDEBUG

    // Sentinels

    assert(!head_.prev && !tail_.next && head_.next && tail_.prev && head_.next->prev == &head_ && tail_.prev->next == &tail_);

    // Forward traversal

    std::size_t list_count{0};
    Node* curr = head_.next;

    while (curr != &tail_) {
        assert(curr && curr->next && curr->prev && curr->prev->next == curr && curr->next->prev == curr);
        ++list_count;
        assert(list_count <= store_.size());

        auto it = store_.find(curr->key);
        assert(it != store_.end());
        assert(it->second.get() == curr);

        curr = curr->next;
    }

    assert(list_count == store_.size());

    // Backward traversal

    list_count = 0;
    curr = tail_.prev;

    while (curr != &head_) {
        assert(curr && curr->next && curr->prev && curr->prev->next == curr && curr->next->prev == curr);
        ++list_count;
        assert(list_count <= store_.size());
        curr = curr->prev;
    }

    assert(list_count == store_.size());

    // Size <= Capacity

    assert(store_.size() <= capacity_);

#endif
}