#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
#include <random>
#include <chrono>
#include <atomic>

#define MAX_PRODUCE 10
#define MAX_QUEUE_SIZE 10
using namespace std;
queue<int> q;
mutex mtx;
condition_variable cv;
atomic<int> curr_produce(0);
// Create a random device (for seeding)
std::random_device rd;  

// Initialize random engine with seed
std::mt19937 gen(rd());  

// Define distribution range [1, 100]
std::uniform_int_distribution<> dist(1, 100);

void produce() {
    while(curr_produce < MAX_PRODUCE) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []() { return q.size() < MAX_QUEUE_SIZE;});
        int random_num = dist(gen); 
        cout << curr_produce<< ". produced " << " curr_produce " << random_num << endl;
        q.push(random_num);
        ++curr_produce;
        cv.notify_all();
        // Random sleep to simulate consumption time
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(dist(gen) * 10));        
    }
}

void consume() {
    while(curr_produce < MAX_PRODUCE) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, []() { return q.size() != 0;});
        int produce = q.front();
        cout << "consume : " << produce << endl;
        q.pop();
        cv.notify_all();  
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(dist(gen) * 10));                
    }
}

int main() {
    thread th_producer(produce) ,th_consumer(consume);

    th_producer.join();
    th_consumer.join();

    return 0;
}