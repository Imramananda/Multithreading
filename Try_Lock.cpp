#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

mutex m;
int a=0;
void f()
{
    if(m.try_lock())
    {
        a++;
        cout<<a<<",";
    }
    m.unlock();
    cout<<endl;

}

int main()
{

    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    return 0;
}