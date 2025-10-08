#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

recursive_mutex rm;
int b=0;
void ff(char c,int l)
{
    if(l<0)
    return;
    rm.lock();
    cout<<"Thread : "<<c<<" b : "<<b++<<endl;
    ff(c,--l);
    rm.unlock();
    cout<<"Thread "<<c<<" unlocked "<<endl;l
}

int main()
{

    cout<<"Main thread"<<endl;
    thread t1(ff,'1',10);
    thread t2(ff,'2',10);
    t1.join();
    t2.join();
    return 0;
}