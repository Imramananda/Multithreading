#include<iostream>
#include<thread>
#include<future>
#include<chrono>

using namespace std;

int findOdd(int start,int end)
{
    int oddSum=0;
    for(int i=start;i<=end;i++)
    {
        if(i&1)
        oddSum+=i;
    }
return oddSum;
}

int main()
{
    cout<<"Thread : "<<thread::hardware_concurrency()<<endl;
    int start=0, end=5;
    cout<<"It is main thread "<<endl;
    //future<int>f=async(launch::deferred,findOdd,start,end); // blocking it will not create the task
    future<int>f=async(launch::async,findOdd,start,end);
    cout<<"waiting for result "<<endl;
    cout<<"OddSum : "<<f.get()<<endl;
return 0;
}