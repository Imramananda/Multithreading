#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

mutex m;
void f()
{
    lock_guard<mutex>l(m);
    cout<<" Thread ID : "<<this_thread::get_id()<<endl;
    for(int i=0;i<=5;i++)
    {
        cout<<"I -: "<<i<<endl;
    }
    cout<<"Function execution completed"<<endl;
}
int main()
{
    cout<<"Main function "<<endl;
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    cout<<"End of main function "<<endl;
    return 0;
}