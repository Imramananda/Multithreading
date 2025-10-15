
// Concept: Thread-safe counter using mutex
// Demonstrates how to protect shared resources in multi-threaded environments.

#include <iostream>
#include <thread>
#include <mutex>

class Counter {
    int value;
    std::mutex m;
public:
    Counter() : value(0) {}
    void increment() {
        std::lock_guard<std::mutex> lock(m); // Lock the mutex to ensure thread safety also unlock it automatically 
        // when lock goes out of scope
        ++value;
    }
    int get() {
        std::lock_guard<std::mutex> lock(m);
        return value;
    }
};

int main() {
    Counter c;
    std::thread t1([&]() { for (int i = 0; i < 10000; ++i) c.increment(); });// Create thread to increment counter
    // Create another thread to increment counter
    std::thread t2([&]() { for (int i = 0; i < 10000; ++i) c.increment(); });
    t1.join(); t2.join();
    std::cout << "Counter: " << c.get() << std::endl; // Expected output: Counter: 20000
    // The counter should be thread-safe and reach 20000 after both threads finish execution
    return 0;
}
