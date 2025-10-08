#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <deque>

using namespace std;
mutex m;
condition_variable cv;
deque<int> buffer;
int maxBufferSize = 50;

void producer(int v)
{
    while(v)
    {
    unique_lock<mutex> ul(m);
    cv.wait(ul, []
            { return buffer.size() < maxBufferSize; });
    buffer.push_back(v);
    cout << "Produced : " << v << endl;
    v--;
    ul.unlock();
    cv.notify_one();
    }
}

void consumer()
{
    while (true)
    {
        unique_lock<mutex> ul(m);
        cv.wait(ul, []
                { return buffer.size() > 0; });
        int val = buffer.back();
        buffer.pop_back();
        cout << "Consumed: " << val << endl;
        ul.unlock();
        cv.notify_one();
    }
}

int main()
{
    cout << "Main thread " << endl;
    thread t1(producer, 100);
    thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
    return 0;
}