#include <iostream>
#include <thread>
#include <semaphore>
std::counting_semaphore<3> sem(3); // max 3 permits

void worker(int id) {
    std::cout << "Thread " << id << " waiting...\n";
    sem.acquire(); // try to acquire permit
    std::cout << "Thread " << id << " got permit.\n";

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Thread " << id << " releasing permit.\n";
    sem.release(); // release permit
}

int main() {
    std::thread threads[6];
    for (int i = 0; i < 6; i++) {
        threads[i] = std::thread(worker, i+1);
    }
    for (auto& t : threads) t.join();
}
