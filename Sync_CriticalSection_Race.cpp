#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

int balance = 0;
mutex m;
void addMoney(int val)
{
    balance = balance + val;
}

int main()
{
    thread t1(addMoney, 100);
    thread t2(addMoney, 200);
    t1.join();
    t2.join();
    cout << "Final Balance : " << balance << endl;
    return 0;
}