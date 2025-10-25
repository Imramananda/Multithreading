#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <queue>

using namespace std;

mutex m;
condition_variable cv;
queue<int> q;
atomic<bool> finished = false;
void producer()
{
    for (int i = 1; i <= 4; i++)
    {
        {
            unique_lock<mutex> l(m);
            q.push(i);
        } // release lock before notify/printing
        cv.notify_one();
        cout << "Producer - " << i << endl;
    }
    finished.store(true);
    cv.notify_all();
}

void consumer()
{
    while (true)
    {
        unique_lock<mutex> l(m);
        cv.wait(l, [] { return !q.empty() || finished.load(); });

        // If queue is empty and producer finished -> exit
        if (q.empty() && finished.load())
            break;

        int consume = q.front();
        q.pop();

        // do printing outside the lock to avoid holding mutex during I/O
        l.unlock();
        cout << "consumed - " << consume << endl;
    }

}

int main()
{
    cout << "Rama main : " << endl;
    thread t1(producer);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}