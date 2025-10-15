/*
this code is part of a C++ project that demonstrates basic multithreading
using the C++17 standard.

here is a simple example that creates two threads,
each printing a message to the console.

*/
#include <iostream>
#include <thread>

void printHello(int id) {
    std::cout << "Hello from thread " << id << std::endl;
}

int main() {
    std::thread t1(printHello, 1);
    std::thread t2(printHello, 2);
    t1.join();
    t2.join();
    return 0;
}
