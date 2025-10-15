/*
==========================================================
Concept: Advanced Thread Pool in C++
==========================================================
 Problem:
    - In multithreaded programming, creating and destroying threads 
      for each small task is expensive.
    - We want to process multiple tasks concurrently but avoid 
      the overhead of creating too many threads.

 Solution:
    - A **Thread Pool** is a fixed set of worker threads that wait 
      for tasks from a queue.
    - When a new task arrives, it is pushed into the queue.
    - One of the idle worker threads picks up the task and executes it.
    - This avoids thread creation overhead and gives controlled concurrency.

 Key Concepts in This Implementation:
    1. **Workers (std::thread)** → a fixed number of threads that continuously 
       wait for work to do.
    2. **Task Queue (std::queue<std::function<void()>>)** → stores pending tasks.
    3. **Mutex + Condition Variable** → synchronizes access to the queue 
       and lets workers sleep when there are no tasks.
    4. **Atomic stop flag** → signals all threads to stop when the pool is 
       being destroyed.

----------------------------------------------------------
This program:
    - Creates a pool of 4 worker threads.
    - Enqueues 8 tasks (printing messages).
    - Each worker thread will grab tasks and execute them.
    - After execution, the destructor cleans up all threads gracefully.
----------------------------------------------------------
*/

#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class ThreadPool {
    std::vector<std::thread> workers;                      // Worker threads
    std::queue<std::function<void()>> tasks;               // Task queue
    std::mutex queue_mutex;                                // Mutex to protect the queue
    std::condition_variable condition;                     // Condition variable for task notification
    std::atomic<bool> stop;                                // Stop flag for graceful shutdown

public:
    // Constructor: initializes worker threads
    ThreadPool(size_t threads) : stop(false) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    {
                        // Lock queue while accessing tasks
                        std::unique_lock<std::mutex> lock(queue_mutex);

                        // Wait until there is a task OR stop is true
                        condition.wait(lock, [this] { 
                            return stop || !tasks.empty(); 
                        });

                        // Exit if stopping and no tasks are left
                        if (stop && tasks.empty()) 
                            return;

                        // Get the next task
                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    // Execute the task outside the lock
                    task();
                }
            });
        }
    }

    // Enqueue a new task into the task queue
    template<class F>
    void enqueue(F&& f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f)); // Add new task
        }
        condition.notify_one(); // Notify one worker thread
    }

    // Destructor: signals all threads to stop and joins them
    ~ThreadPool() {
        stop = true;                // Set stop flag
        condition.notify_all();     // Wake all waiting threads

        // Join all worker threads
        for (std::thread &worker : workers) 
            worker.join();
    }
};

int main() {
    ThreadPool pool(4); // Create a pool of 4 worker threads

    // Enqueue 8 tasks
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i 
                      << " is running in thread " 
                      << std::this_thread::get_id() << "\n";
        });
    }

    // Give time for tasks to complete before main exits
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
