
// Concept: Resolving deadlock using std::lock
// Ensures consistent locking of multiple mutexes to avoid deadlock.

#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1, m2;

void task() {
    std::lock(m1, m2);
    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock); // adopt_lock indicates that the mutex is already locked
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
    std::cout << "Task completed without deadlock\n";
}

int main() {
    std::thread t1(task);
    std::thread t2(task);
    t1.join(); t2.join();
    return 0;
}
