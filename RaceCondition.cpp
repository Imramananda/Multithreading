#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

int I=0;
mutex m;
void add()
{
    m.lock();
    while(I<5)
    cout<<"I : " << I++<<endl; // race condition n and while loop is critical section
    m.unlock();
}

int main()
{
    thread t1(add);
    thread t2(add);
    t1.join();
    t2.join();
    cout<<"Value I "<<I<<endl;
    return 0;
}

// both r trying to modify I , that is race condition and while loop is critical section