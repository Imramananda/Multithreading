#include <iostream>
#include <condition_variable>
#include <mutex>
#include <thread>
using namespace std;

condition_variable cv;
mutex m;
int cur = 1;
bool isOddTurn = true;

void printOdd(const int &n)
{
    while (cur <= n)
    {
        unique_lock<mutex> l(m);
        cv.wait(l, [&]{ return cur > n || (cur % 2 != 0 && isOddTurn); });
        
        if (cur <= n)
        {
            cout << "Odd  : " << cur << endl;
            cur++;
            isOddTurn = false;
        }
        cv.notify_one();
    }
}

void printEven(const int &n)
{
    while (cur <= n)
    {
        unique_lock<mutex> l(m);
        cv.wait(l, [&]{ return cur > n || (cur % 2 == 0 && !isOddTurn); });
        
        if (cur <= n)
        {
            cout << "Even : " << cur << endl;
            cur++;
            isOddTurn = true;
        }
        cv.notify_one();
    }
}

int main()
{
    cout << "Starting alternate odd-even printing up to 20..." << endl;
    
    thread t1(printOdd, 20);
    thread t2(printEven, 20);
    
    t1.join();
    t2.join();
    
    return 0;
}
