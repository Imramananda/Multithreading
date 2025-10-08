#include<iostream>
#include<thread>
#include<future>
using namespace std;

void ff(promise<int>&&p)
{
    cout<<"Thread 1"<<endl;
    this_thread::sleep_for(chrono::seconds(2));
    p.set_value(5000);
}

int main()
{
    cout<<"Main thread"<<endl;
    promise<int>p;
    future<int>f = p.get_future();

    thread t1(ff,move(p));
    cout<<"wainting for result "<<endl;
  
    cout<<"Value recieved : "<<f.get()<<endl;
      t1.join();
    return 0;
}