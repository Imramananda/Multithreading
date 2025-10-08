#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex m;
condition_variable cv;
int balance = 0;

void addMoney(int money)
{
    lock_guard<mutex> obj(m);
    balance = balance + money;
    cout << "Amount added current balance " << balance << endl;
    cv.notify_one();
}

void withdrowlMoney(int money)
{
    unique_lock<mutex> obj(m);
    cv.wait(obj,[]{ return (balance != 0) ? true : false; });
    if (balance >= money)
    {
        balance = balance - money;
        cout << "Amount deducted : " << money << endl;
    }
    else
    {
        cout << "Amount cant be deducted current balance is less than " << money << endl;
    }
    cout << "Available Balance is : " << balance << endl;
}

int main()
{
    cout << "Main thread " << endl;
    thread t1(withdrowlMoney, 500);
    thread t2(addMoney, 500);
    t1.join();
    t2.join();
    return 0;
}