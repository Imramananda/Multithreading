#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#define MAX_CNT 20
using namespace std;

mutex mtx;
condition_variable cv;
atomic<int> current_cnt(1);
atomic<bool> is_odd_turn(true);

void print_even() {
    while(current_cnt < MAX_CNT) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return !is_odd_turn.load();});
        cout<<"Even " <<current_cnt.load() << endl;
        ++current_cnt;
        is_odd_turn = true;
        cv.notify_all();
    }
}

void print_odd() {
    while(current_cnt < MAX_CNT) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [&]() { return is_odd_turn.load();});
        cout<<"Odd "<< current_cnt.load() << endl;
        ++current_cnt;
        is_odd_turn = false;
        cv.notify_all();
    }
}



int main() {
    thread th_even(print_even), th_odd(print_odd);

    th_odd.join();
    th_even.join();
    return 0;
}