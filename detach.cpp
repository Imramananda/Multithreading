#include<iostream>
#include<thread>
#include<chrono>
using namespace std;
void fn()
{
    cout<<"Aryan fn"<<endl;
    this_thread::sleep_for(chrono::seconds(5));
    cout<<"After fn "<<endl;
}

int main()
{
    cout<<"Aryan mainfn "<<endl;
    thread t(fn);
    //t.join();
    //t.join(); // double join terminate the program
    t.detach();
    // if(t.joinable())
    // {
    //     cout<<"inside"<<endl;
    //     t.join();
    // }
    cout<<"after join"<<endl;

    return 0;
}