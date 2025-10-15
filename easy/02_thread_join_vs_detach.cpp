/*

this code demonstrates the difference between `join` and `detach` in C++ threads.
When using `join`, the main thread waits for the background thread to finish.
When using `detach`, the background thread runs independently, and the main thread does not wait for it.
If the main thread exits before the detached thread finishes, the detached thread may be terminated prematurely.

*/
#include <iostream>
#include <thread>

void task() {
    std::cout << "Running task in background\n";
}

int main() {
    std::thread t(task);
    //t.join(); // Join the thread to wait for it to finish
    
    t.detach(); // Detach the thread to run independently till the main thread exits
    return 0;
}
