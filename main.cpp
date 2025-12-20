#include "LRUCache.h"
#include <iostream>
#include <cassert>
#include <optional>

std::size_t tests_passed = 0;
const std::size_t total_tests = 33;

void message() {
    ++tests_passed;
    std::cout << "|\t(" << tests_passed << '/' << total_tests << ")\tTEST(S) PASSED\t\t|\n";
}

int main() {
    const std::size_t capacity = 10;

    LRUCache lru(capacity);

    std::cout << "\nBEGINNING TESTS...\n ";

    for (std::size_t i{0}; i < 39; ++i) std::cout << '_';
    std::cout << " \n|";

    for (std::size_t i{0}; i < 39; ++i) std::cout << ' ';
    std::cout << "|\n";

    // Tests while empty

    assert(!lru.size());
    message();

    assert(lru.empty());
    message();

    assert(lru.get(1) == std::nullopt);
    message();

    assert(!lru.contains(1));
    message();

    assert(!lru.erase(1));
    message();

    // Tests while size == 2

    lru.put(1, 100);
    lru.put(2, 200);

    assert(lru.size() == 2);
    message();

    assert(lru.get(1) == 100);
    message();

    assert(lru.get(2) == 200);
    message();

    assert(lru.contains(1));
    message();

    assert(lru.erase(1));
    message();

    assert(!lru.contains(1));
    message();

    assert(lru.contains(2));
    message();

    lru.clear();

    assert(!lru.contains(2));
    message();

    assert(lru.empty());
    message();

    // Tests for capacity overflow and eviction

    for (std::size_t i{1}; i <= capacity; ++i) lru.put(i, i * 100);

    assert(lru.size() == capacity);
    message();

    lru.put(capacity + 1, (capacity + 1) * 100);

    assert(lru.get(11) == 1100);
    message();

    assert(lru.get(1) == std::nullopt);
    message();

    assert(!lru.contains(1));
    message();

    assert(!lru.erase(1));
    message();

    for (std::size_t i{2}; i <= (capacity + 1) / 2; ++i) {
        lru.get(i);
    }

    assert(lru.contains(((capacity + 1) / 2) + 1));
    message();

    lru.put(capacity + 2, (capacity + 2) * 100);

    assert(!lru.contains(((capacity + 1) / 2) + 1));
    message();

    for (std::size_t i{2}; i <= capacity + 2; ++i) {
        if (i == ((capacity + 1) / 2) + 1) continue;
        assert(lru.get(i) != std::nullopt);
    }
    message();

    lru.put(((capacity + 1) / 2) + 1, (((capacity + 1) / 2) + 1) * 100);

    assert(!lru.contains(2));
    message();

    assert(lru.erase(((capacity + 1) / 2) + 1));
    message();

    assert(lru.get(3) == 300);
    message();

    lru.put(3, 600);

    assert(lru.get(3) == 600);
    message();

    assert(!lru.empty());
    message();

    lru.clear();

    for (std::size_t i{1}; i <= capacity + 2; ++i) {
        assert(lru.get(i) == std::nullopt && !lru.contains(i));
    }
    assert(!lru.size());
    message();

    assert(lru.empty());
    message();

    // Tests for LRU with capacity 0

    LRUCache empty_lru(0);

    empty_lru.put(1, 1);

    assert(empty_lru.get(1) == std::nullopt);
    message();

    assert(!empty_lru.contains(1));
    message();

    assert(!empty_lru.size());
    message();

    assert(empty_lru.empty());
    message();

    std::cout << '|';
    for (std::size_t i{0}; i < 39; ++i) std::cout << '_';
    std::cout << "|\n\nTESTS FINISHSED\n";

    return 0;
}