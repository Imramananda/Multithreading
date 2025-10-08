#include<iostream>
#include<memory>
#include<thread>

using namespace std;

void f(int n)
{
cout<<"Rama f "<<n<<endl;
}

int main()
{
    thread t1(f,10);
    t1.join(); // main thread wait t1 to finish

    return 0;
}