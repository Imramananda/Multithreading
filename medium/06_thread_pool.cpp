/*
Medium Problem 06: Thread Pool
--------------------------------
Concept:
A thread pool allows reusing a fixed number of worker threads 
to execute multiple tasks, avoiding the overhead of creating 
and destroying threads for each task.

Key Points:
- Use a queue to store tasks.
- Use condition_variable to notify workers of new tasks.
- Each worker thread keeps running and picks tasks from the queue.
*/

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks; // Task queue
    std::mutex m;
    std::condition_variable cv;
    bool stop = false;

public:
    ThreadPool(size_t n) {
        for (size_t i = 0; i < n; i++) {
            workers.emplace_back([this] { // Worker thread function
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(m);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(m);
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(m);
            stop = true;
        }
        cv.notify_all();
        for (auto &w : workers) w.join();
    }
};

int main() {
    ThreadPool pool(3);
    for (int i = 0; i < 5; i++) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " executed by thread "
                      << std::this_thread::get_id() << "\n";
        });
    }
}
