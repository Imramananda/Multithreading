#include <iostream>
#include <thread>

using namespace std;

class A
{
public:
    void func()
    {
        cout << "Func " << endl;
    }
};

int main()
{
    A obj;
    cout<<"Main function"<<endl;
    thread t(&A::func,&obj);
    t.join();
    return 0;
}
