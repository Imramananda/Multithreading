#include<iostream>
#include<thread>
using namespace std;

int main()
{
    auto fun =[](int x){

        while(x-- > 0)
        cout<<x<<endl;
    };
    thread t(fun,3); // lambda expression
    t.join();
    return 0;
}