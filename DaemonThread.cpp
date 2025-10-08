#include<iostream>
#include<thread>

using namespace std;

void f(int a)
{
    cout<<"Function : "<<a<<endl;

}

int main()
{
    cout<<"Main "<<endl;
    thread t(f,10);
    //t.detach(); // no need to wait for t main thread can excute and proceed further 
    t.join(); // wait here until t get comleted 
    cout<<"Rama "<<endl;
    return 0;
}
