/*

This code demonstrates a simple multithreading example in C++ where two threads increment a shared counter.
The expected output is that the counter should reach 20000 after both threads finish execution.
However, due to the lack of synchronization, the output may vary, leading to potential race conditions.
To ensure thread safety, consider using mutexes or atomic variables.

*/
#include <iostream>
#include <thread>

int counter = 0;

void increment() {
    for (int i = 0; i < 10000; ++i) ++counter;
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);
    t1.join(); t2.join();
    std::cout << "Counter: " << counter << std::endl; // Expected output: Counter: 20000
    // Note: The output may vary if the increment function is not synchronized properly.
    // To ensure thread safety, consider using mutexes or atomic variables.
    return 0;
}
