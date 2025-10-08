#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

mutex m1, m2;
// deadlock situation, dont change the order of mutex lock
void thread1()
{
    m1.lock();
    std::this_thread::sleep_for(chrono::seconds(1));
    m2.lock();
    cout << "Critical section for thread 1" << endl;
    m1.unlock();
    m2.unlock();
}
void thread2()
{
    m2.lock();
    std::this_thread::sleep_for(chrono::seconds(1));
    m1.lock();
    cout << "Critical section for thread 2" << endl;
    m2.unlock();
    m1.unlock();
}

int main()
{

    thread t1(thread1);
    thread t2(thread2);
    t1.join();
    t2.join();
    return 0;
}