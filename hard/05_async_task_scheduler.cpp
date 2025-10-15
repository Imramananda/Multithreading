/*
    Concept: Task Scheduler using Priority Queue + Condition Variable

    - This program implements a task scheduler where you can schedule tasks 
      (functions) to run after a certain delay.

    - Internals:
        1. A background worker thread continuously monitors a priority queue 
           (min-heap) of tasks ordered by their scheduled time.
        2. Each task is stored as:
              { execution_time, function_to_execute }
        3. The worker thread waits until the next task’s scheduled time, 
           executes it, then continues.
        4. If no tasks exist, it waits until new tasks are scheduled.
        5. Thread safety is ensured with std::mutex and std::condition_variable.
        6. When the scheduler is destroyed, the worker thread stops gracefully.

    - Usage in main():
        * Schedule Task A after 500 ms
        * Schedule Task B after 100 ms
        * The worker thread executes them in the right order (B first, then A)
*/

#include <iostream>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Comparator for priority queue (min-heap based on execution time)
struct TaskComparator {
    bool operator()(const std::pair<std::chrono::steady_clock::time_point, std::function<void()>>& a,
                    const std::pair<std::chrono::steady_clock::time_point, std::function<void()>>& b) const {
        return a.first > b.first;  // Earlier time has higher priority
    }
};

class TaskScheduler {
    using Task = std::pair<std::chrono::steady_clock::time_point, std::function<void()>>;

    // Priority queue: tasks sorted by execution time (soonest first)
    std::priority_queue<
        Task,
        std::vector<Task>,
        TaskComparator
    > tasks;

    std::mutex m;                  // Mutex to protect shared data
    std::condition_variable cv;    // Condition variable for thread signaling
    bool stop = false;             // Stop flag for graceful shutdown
    std::thread worker;            // Background worker thread

public:
    TaskScheduler() {
        // Start the worker thread
        worker = std::thread([this]() {
            std::unique_lock<std::mutex> lock(m);
            while (!stop) {
                if (!tasks.empty()) {
                    auto now = std::chrono::steady_clock::now();
                    auto& [time, task] = tasks.top(); // Peek earliest task
                    if (now >= time) {
                        // Execute task if scheduled time has arrived
                        task();
                        tasks.pop();
                    } else {
                        // Wait until the scheduled time of the next task
                        cv.wait_until(lock, time);
                    }
                } else {
                    // If no tasks, wait until new task is added
                    cv.wait(lock);
                }
            }
        });
    }

    // Schedule a task to run after a given delay
    void schedule(std::function<void()> task, std::chrono::milliseconds delay) {
        std::unique_lock<std::mutex> lock(m);
        tasks.emplace(std::chrono::steady_clock::now() + delay, std::move(task));
        cv.notify_one(); // Wake worker thread
    }

    ~TaskScheduler() {
        {
            std::unique_lock<std::mutex> lock(m);
            stop = true;
        }
        cv.notify_all();   // Wake worker to exit
        worker.join();     // Join background thread
    }
};

int main() {
    TaskScheduler scheduler;

    scheduler.schedule([] { std::cout << "Task A executed\n"; }, std::chrono::milliseconds(500));
    scheduler.schedule([] { std::cout << "Task B executed\n"; }, std::chrono::milliseconds(100));

    // Wait long enough for tasks to run
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
