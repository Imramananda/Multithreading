#include<iostream>
#include<thread>
#include<atomic>
#include<mutex>
using namespace std;

mutex m;
int counter=0;

//race condition
// void f()
// {
//     for(int i=1;i<=1000;i++)
//     counter++;

// }

// fix with mutex
mutex m;
atomic<int> count =0;
void f()
{
    for(int i=1;i<=1000;i++)
    {
        lock_guard<mutex>l(m);
        counter++;
    }
}

// fix using atomic;

int main()
{
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();

    cout<<"Counter : "<<counter<<endl;
    return 0;
}