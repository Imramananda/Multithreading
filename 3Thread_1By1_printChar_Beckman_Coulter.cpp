#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
using namespace std;

mutex m;
char a[] = "abcdefghijkl";
int s = sizeof(a) / sizeof(a[0]);
int cond = 0;
int i = 0;
condition_variable cv;
void f() {
    while (i < s - 1) {
        unique_lock<mutex> l(m);
        cv.wait(l, []() { return cond == 0; });
        if (i < s - 1) {
            cout << "From F  : " << a[i++] << endl;
            cond = 1;               // next turn: f1
            cv.notify_all();        // wake up others
        }
    }
}

void f1() {
    while (i < s - 1) {
        unique_lock<mutex> l(m);
        cv.wait(l, []() { return cond == 1; });
        if (i < s - 1) {
            cout << "From F1 : " << a[i++] << endl;
            cond = 2;               // next turn: f2
            cv.notify_all();
        }
    }
}

void f2() {
    while (i < s - 1) {
        unique_lock<mutex> l(m);
        cv.wait(l, []() { return cond == 2; });
        if (i < s - 1) {
            cout << "From F2 : " << a[i++] << endl;
            cond = 0;               // next turn: f
            cv.notify_all();
        }
    }
}
int main()
{
    cout << "Size of array is " << sizeof(a) / sizeof(a[0]) << endl;
    thread t1(f);
    thread t2(f1);
    thread t3(f2);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}