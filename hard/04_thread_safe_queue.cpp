/*
===============================================================
   Concept: Thread-Safe Queue using Mutex + Condition Variable
===============================================================
This program demonstrates a classic **Producer-Consumer problem**
using C++ threads, mutex, and condition_variable.

- ThreadSafeQueue<T>:
    * Provides thread-safe enqueue and dequeue.
    * Uses std::mutex to protect shared state.
    * Uses std::condition_variable to make consumers wait until
      there is data in the queue, instead of busy-waiting.

- Producer thread:
    * Generates numbers and puts them into the queue.

- Consumer thread:
    * Waits (blocks) until data is available in the queue,
      then removes and processes it.

Key Points:
1. `enqueue()` locks the queue, adds an item, and notifies a consumer.
2. `dequeue()` waits until the queue is not empty, then removes an item.
3. `cv.wait(lock, predicate)` handles spurious wake-ups safely.
4. Demonstrates synchronization between multiple threads.
*/

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

// ==========================
// Thread-Safe Queue Template
// ==========================
template<typename T>
class ThreadSafeQueue {
    std::queue<T> queue;             // Standard STL queue to hold data
    std::mutex m;                    // Mutex to protect queue access
    std::condition_variable cv;      // Condition variable for blocking/wakeup

public:
    // Add an item into the queue (called by producer)
    void enqueue(T item) {
        std::lock_guard<std::mutex> lock(m);  // Lock mutex during modification
        queue.push(item);                     // Add new item
        cv.notify_one();                      // Notify one waiting consumer
    }

    // Remove and return an item from the queue (called by consumer)
    T dequeue() {
        std::unique_lock<std::mutex> lock(m); // Lock with unique_lock (needed for cv.wait)
        // Wait until queue is NOT empty (avoid spurious wakeups)
        cv.wait(lock, [this] { return !queue.empty(); });
        T item = queue.front();               // Get front item
        queue.pop();                          // Remove it
        return item;                          // Return to caller
    }
};

// ==========================
// Main: Producer + Consumer
// ==========================
int main() {
    ThreadSafeQueue<int> q;   // Shared queue between producer & consumer

    // Producer thread: pushes 5 numbers into the queue
    std::thread producer([&]() {
        for (int i = 0; i < 5; ++i) {
            q.enqueue(i);                         // Add item to queue
            std::cout << "Produced: " << i << "\n";
        }
    });

    // Consumer thread: pops 5 numbers from the queue
    std::thread consumer([&]() {
        for (int i = 0; i < 5; ++i) {
            int item = q.dequeue();               // Wait until data is available
            std::cout << "Consumed: " << item << "\n";
        }
    });

    // Wait for both threads to finish
    producer.join();
    consumer.join();

    return 0;
}
