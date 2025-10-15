/*
Medium Problem 07: Barrier Synchronization
--------------------------------
Concept:
A barrier ensures that multiple threads reach a certain 
point of execution before any can proceed further.

Here, we use a counter + condition_variable to simulate
a barrier for N threads.
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

class Barrier {
    std::mutex m;
    std::condition_variable cv;
    int count;
    int waiting = 0;

public:
    Barrier(int c) : count(c) {}

    void wait() {
        std::unique_lock<std::mutex> lock(m);
        waiting++;
        if (waiting == count) {
            waiting = 0;
            cv.notify_all(); // release all threads
        } else {
            cv.wait(lock, [this] { return waiting == 0; }); // wait until all threads arrive
        }
    }
};

void worker(Barrier &b, int id) {
    std::cout << "Thread " << id << " reached barrier\n";
    b.wait();
    std::cout << "Thread " << id << " passed barrier\n";
}

int main() {
    const int N = 4; // Number of threads to synchronize at the barrier
    Barrier b(N);
    std::vector<std::thread> threads;

    for (int i = 0; i < N; i++)
        threads.emplace_back(worker, std::ref(b), i);

    for (auto &t : threads) t.join();
}
