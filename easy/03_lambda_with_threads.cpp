/*
This code demonstrates the use of a lambda function to create a thread in C++.
The lambda function prints a message when the thread is executed.

components of the code:
- Defines a main function that creates a thread using a lambda expression.
- Joins the thread to ensure it completes before the program exits.

Advantages:
- Simple and concise syntax for creating threads.
- Allows for inline definition of the thread's behavior.
- Ensures that the main thread waits for the lambda thread to finish execution.

*/
#include <iostream>
#include <thread>

int main() {
    std::thread t([]() {
        std::cout << "Hello from lambda thread\n";
    });
    t.join();
    return 0;
}
