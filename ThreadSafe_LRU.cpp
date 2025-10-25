#include <iostream>
#include <unordered_map>
#include <list>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

class LRU
{
private:
    int capacity;
    list<pair<int, int>> l;
    unordered_map<int, list<pair<int, int>>::iterator> um;

public:
    LRU(int cap = 5) : capacity(cap) {}

public:
    int get(int key)
    {
        lock_guard<mutex> lo(m);
        if (um.find(key) == um.end())
        {
            cout<<"not found "<<endl;
            return -1;
        }
        l.splice(l.begin(), l, um[key]);
        cout<<" found "<<key<<endl;
        return um[key]->second;
    }

    void put(int key, int value)
    {
        lock_guard<mutex> lo(m);
        if (get(key) != -1)
        {
            cout<<"mil gya found "<<endl;
            um[key]->second = value;
            return;
        }
        if (capacity == l.size())
        {
            cout<<" full ho gya "<<endl;
            int lastKey = l.back().first;
            l.pop_back();
            um.erase(lastKey);
        }
        l.emplace_front(key, value);
        um[key] = l.begin();
        cout<<"New ENtry found "<<endl;
    }
};

void writer(LRU& cache, int start)
{
    for(int i = start; i < start + 5; i++) {
        cache.put(i, i * 10);
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void reader(LRU& cache, const vector<int>& keys)
{
    for(int key : keys) {
        cache.get(key);
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main()
{
    LRU cache;
    vector<thread> threads;
    
    // Create writer threads
    threads.emplace_back(writer, ref(cache), 0);  // Writes 0-4
    threads.emplace_back(writer, ref(cache), 5);  // Writes 5-9
    
    // Create reader threads with different keys to read
    vector<int> keys1 = {0, 2, 4, 6, 8};
    vector<int> keys2 = {1, 3, 5, 7, 9};
    threads.emplace_back(reader, ref(cache), keys1);
    threads.emplace_back(reader, ref(cache), keys2);
    
    // Wait for all threads to complete
    for(auto& t : threads) {
        t.join();
    }
    
    cout << "All threads completed" << endl;
    return 0;
}