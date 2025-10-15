
// Concept: Deadlock scenario using multiple mutexes
// Demonstrates how threads can enter a deadlock if mutexes are acquired in inconsistent order.
// task1 locks m1 then m2, while task2 locks m2 then m1, leading to potential deadlock.

#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2;

void task1() {
    std::lock_guard<std::mutex> lock1(m1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(m2);
    std::cout << "Task1 completed\n";
}

void task2() {
    std::lock_guard<std::mutex> lock2(m2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock1(m1);
    std::cout << "Task2 completed\n";
}

int main() {
    std::thread t1(task1);
    std::thread t2(task2);
    t1.join(); t2.join();
    return 0;
}
