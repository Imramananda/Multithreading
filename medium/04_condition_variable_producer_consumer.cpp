
// Concept: Producer-consumer problem using condition_variable
// Uses a queue shared between producer and consumer threads synchronized using condition_variable.

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;

void producer() {
    for (int i = 1; i <= 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex to protect shared queue
        q.push(i);
        std::cout << "Produced: " << i << "\n";
        cv.notify_one(); // Notify one waiting consumer
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Simulate work
    }
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx); // Lock the mutex to protect shared queue
        cv.wait(lock, [] { return !q.empty(); }); // Wait until queue is not empty
        int item = q.front(); q.pop();
        std::cout << "Consumed: " << item << "\n";
        if (item == 5) break;
    }
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}
