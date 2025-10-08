#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

mutex m;
int b=0;
void ff(char c,int l)
{
    lock_guard<mutex> lock(m);
    for(int i=0;i<l;i++)
    {
        b++;
    cout<<"Thread : "<<c<<" b : "<<b++<<endl;
    }
    cout<<"Thread "<<c<<" unlocked "<<endl;
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