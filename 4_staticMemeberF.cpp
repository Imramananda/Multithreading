#include <iostream>
#include <thread>

using namespace std;

class A
{
public:
    static void func()
    {
        cout << "Func " << endl;
    }
};

int main()
{
    A obj;
    cout<<"Main function"<<endl;
    thread t(&A::func);
    t.join();
    return 0;
}
