#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <memory>
#include <cstddef>
#include <optional>

class LRUCache {
    public:
        explicit LRUCache(std::size_t capacity);
        LRUCache(const LRUCache&) = delete;
        LRUCache& operator=(const LRUCache&) = delete;
        ~LRUCache() = default;

        std::optional<int> get(int key);
        void put(int key, int value);
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool contains(int key) const noexcept;
        [[nodiscard]] bool erase(int key);
        void clear() noexcept;

    private:
        struct Node {
            Node* next;
            Node* prev;
            int key;
            int value;
        };

        Node head_;
        Node tail_;
        std::unordered_map<int, std::unique_ptr<Node>> store_;
        std::size_t capacity_;

        void detach(Node* n);
        void attach_front(Node* n);
        void move_to_front(Node* n);
        void evict_lru();
        void check_invariants() const;
};

#endif // LRUCACHE_H