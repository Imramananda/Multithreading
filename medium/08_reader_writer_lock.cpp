/*
Medium Problem 08: Reader-Writer Lock
--------------------------------
Concept:
Multiple readers can access shared data simultaneously,
but writers need exclusive access.

Here, we use `std::shared_mutex` introduced in C++17.
*/

#include <iostream>
#include <thread>
#include <shared_mutex>
#include <vector>
#include <mutex>

std::shared_mutex rw_mutex;
int shared_data = 0;

void reader(int id) {
    std::shared_lock lock(rw_mutex); // Shared lock for readers
    std::cout << "Reader " << id << " reads value: " << shared_data << "\n";
}

void writer(int id) {
    std::unique_lock lock(rw_mutex); // Exclusive lock for writers
    shared_data += id;
    std::cout << "Writer " << id << " updated value to: " << shared_data << "\n";
}

int main() {
    std::vector<std::thread> threads;
    threads.emplace_back(writer, 1);
    threads.emplace_back(reader, 1);
    threads.emplace_back(reader, 2);
    threads.emplace_back(writer, 2);
    threads.emplace_back(reader, 3);

    for (auto &t : threads) t.join(); // Wait for all threads to finish
}
