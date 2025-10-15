/**
 * Medium Problem 5: Thread-Safe Vector
 *
 * Concept:
 * =========
 * In multithreaded programs, concurrent access to `std::vector`
 * can lead to data races and undefined behavior.
 *
 * Solution:
 * =========
 * We create a wrapper class `ThreadSafeVector` that internally
 * protects the underlying `std::vector` with a `std::mutex`.
 * 
 * Features:
 * - Safe push_back from multiple threads
 * - Safe read (get element)
 * - Thread-safe size retrieval
 *
 * Key Concepts:
 * - `std::mutex` for mutual exclusion
 * - RAII via `std::lock_guard`
 * - Encapsulation of thread-safety
 */

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

class ThreadSafeVector {
private:
    std::vector<int> data;
    std::mutex m;

public:
    void push_back(int value) {
        std::lock_guard<std::mutex> lock(m);
        data.push_back(value);
    }

    int get(size_t index) {
        std::lock_guard<std::mutex> lock(m);
        if (index < data.size()) {
            return data[index];
        }
        throw std::out_of_range("Index out of range");
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(m);
        return data.size();
    }
};

void writer(ThreadSafeVector &vec, int start) {
    for (int i = 0; i < 100; ++i) {
        vec.push_back(start + i);
    }
}

int main() {
    ThreadSafeVector vec;

    // Launch multiple writer threads
    std::thread t1(writer, std::ref(vec), 0);
    std::thread t2(writer, std::ref(vec), 1000);
    std::thread t3(writer, std::ref(vec), 2000);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final vector size: " << vec.size() << "\n"; // Should be 300

    // Accessing some elements
    std::cout << "First element: " << vec.get(0) << "\n";
    std::cout << "Last element: " << vec.get(vec.size() - 1) << "\n";

    return 0;
}
