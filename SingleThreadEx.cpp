#include<iostream>
#include<thread>
#include<chrono>
#include<algorithm>

using namespace std;
using namespace std::chrono;

typedef unsigned long long ull;

ull  evenSum =0;
ull oddSum=0;
void evenSumf(int start , int end)
{
    for(int i=start;i<=end;i++)
    {
       if ((i & 1) == 0) // even
        evenSum = evenSum+i;
    }
}
void oddSumf(int start , int end)
{
    for(int i=start;i<=end;i++)
    {
        if ((i & 1) == 1) // odd
        oddSum = oddSum+i;
    }
}
int main()
{
    int start =0; 
    int end =1900000000000;
    auto startTime = high_resolution_clock::now();
    thread t1(evenSumf,start,end);  // function pointer
    thread t2(oddSumf,start,end); //function pointer
    t1.join();
    t2.join();
    // oddSumf(start,end);
    // evenSumf(start,end);
    auto endTime = high_resolution_clock::now();
    auto duration =duration_cast<microseconds>(endTime-startTime);
    cout << "Duration : " << duration.count() /1000000 << " sec " << endl; // it is taking 4 sec
    cout<<"Rma evenSum : "<<evenSum<<endl;
    cout<<"Ram oddSum : "<<oddSum<<endl;
    return 0;
}