/*

This code demonstrates a simple multithreading example in C++ where two threads increment a shared counter.
The expected output is that the counter should reach 20000 after both threads finish execution.
lock_guard is used to ensure that the increment operation is thread-safe.

lock_guard is a RAII-style mechanism that locks the mutex when it is created and automatically unlocks it when it goes out of scope.

*/
#include <iostream>
#include <thread>
#include <mutex>

int counter = 0;
std::mutex mtx;

void increment() {
    for (int i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex to ensure thread safety
        ++counter;
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join(); // Wait for both threads to finish
    // Expected output: Counter: 20000
    std::cout << "Counter: " << counter << std::endl;
    return 0;
}
