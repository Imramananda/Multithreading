// Concept: Lock-free stack using atomic operations (Multithreaded Demo)
// ----------------------------------------------------------------------
// This program demonstrates a lock-free stack where multiple threads
// can push and pop concurrently without using mutexes.
// We use compare-and-swap (CAS) to achieve atomic updates on the stack head.
// 
// Key properties:
// - Lock-free: no blocking, all threads make progress.
// - Not wait-free: some threads may retry multiple times if CAS fails.
// - Useful in high-concurrency systems where mutexes cause contention.

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

template<typename T>
class LockFreeStack {
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };

    std::atomic<Node*> head;

public:
    LockFreeStack() : head(nullptr) {}

    void push(T value) {
        Node* new_node = new Node(value);
        new_node->next = head.load();

        // Retry until CAS succeeds
        while (!head.compare_exchange_weak(new_node->next, new_node));
    }

    bool pop(T& result) {
        Node* old_head = head.load();
        // Retry until CAS succeeds or stack empty
        while (old_head && 
               !head.compare_exchange_weak(old_head, old_head->next));
        
        if (old_head) {
            result = old_head->data;
            delete old_head;
            return true;
        }
        return false;
    }
};

// ------------------- Multithreaded Demo -------------------

LockFreeStack<int> stack;

void producer(int id, int count) {
    for (int i = 0; i < count; i++) {
        int value = id * 100 + i;  // Unique values per producer
        stack.push(value);
        std::cout << "Producer " << id << " pushed: " << value << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void consumer(int id, int count) {
    for (int i = 0; i < count; i++) {
        int value;
        if (stack.pop(value)) {
            std::cout << "Consumer " << id << " popped: " << value << "\n";
        } else {
            std::cout << "Consumer " << id << " found stack empty.\n";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

int main() {
    const int numProducers = 3;
    const int numConsumers = 3;
    const int opsPerProducer = 5;

    std::vector<std::thread> threads;

    // Start producers
    for (int i = 0; i < numProducers; i++) {
        threads.emplace_back(producer, i + 1, opsPerProducer);
    }

    // Start consumers
    for (int i = 0; i < numConsumers; i++) {
        threads.emplace_back(consumer, i + 1, opsPerProducer);
    }

    // Join all threads
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All threads finished.\n";
    return 0;
}
