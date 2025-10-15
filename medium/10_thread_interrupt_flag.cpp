/*
Medium Problem 10: Thread Interruption with Atomic Flag
--------------------------------
Concept:
C++ threads cannot be interrupted directly.
We simulate interruption using an atomic flag.
*/

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

std::atomic<bool> stop_flag(false); // Atomic flag to signal stop

void worker() {
    while (!stop_flag) {
        std::cout << "Working...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "Worker stopped gracefully.\n";
}

int main() {
    std::thread t(worker);
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Let it work for 2 seconds

    stop_flag = true; // request stop
    t.join();
}
