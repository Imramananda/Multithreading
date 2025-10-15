/*
Medium Problem 09: Future and Promise
--------------------------------
Concept:
`std::promise` sets a value, `std::future` retrieves it.
This allows passing results between threads safely.

It is useful for synchronizing data between threads. 
Key Points:
- `std::promise` is used to set a value that will be available in the future.
- `std::future` is used to retrieve the value set by the promise.
- The promise and future are linked; when the promise sets a value, the future can retrieve it.
- This is particularly useful for passing results from a worker thread back to the main thread.

*/

#include <iostream>
#include <thread>
#include <future>

void computeSquare(std::promise<int> p, int x) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate long computation
    p.set_value(x * x); // Set the result in the promise
}

int main() {
    std::promise<int> p;
    std::future<int> f = p.get_future(); // Get the future associated with the promise

    std::thread t(computeSquare, std::move(p), 5);

    std::cout << "Waiting for result...\n";
    std::cout << "Square = " << f.get() << "\n";

    t.join();
}
