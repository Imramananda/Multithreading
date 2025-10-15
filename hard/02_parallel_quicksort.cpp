// Concept: Parallel Quicksort using Threads
// -----------------------------------------
// Quicksort is a divide-and-conquer sorting algorithm. Normally, it recursively sorts
// the left and right partitions in a single thread. To speed things up on multi-core 
// systems, we can use threads to sort large partitions in parallel.
//
// Idea:
// 1. Pick a pivot element (here, the last element of the array).
// 2. Partition the array around the pivot.
// 3. Recursively sort left and right halves.
// 4. If a partition is "large enough" (threshold = 1000 elements), we launch a new thread
//    to sort it in parallel. Otherwise, we sort it sequentially (to avoid excessive thread overhead).
//
// Key Concepts Demonstrated:
// - Recursive multithreading.
// - Controlled thread creation (avoid creating too many threads).
// - Joining threads safely to prevent resource leaks.

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

// Function: parallel_quicksort
// -----------------------------
// Sorts the subarray arr[left..right] using quicksort with optional threading.
void parallel_quicksort(std::vector<int>& arr, int left, int right) {
    // Base case: if range is invalid or of size 1, nothing to sort
    if (left >= right) return;

    // Partitioning step
    int pivot = arr[right];   // Choose the last element as pivot
    int i = left;             // Index for smaller element
    for (int j = left; j < right; ++j) {
        if (arr[j] < pivot) {
            std::swap(arr[i], arr[j]);
            ++i;
        }
    }
    std::swap(arr[i], arr[right]); // Place pivot in correct position

    // Threads for left and right partitions
    std::thread t1, t2;

    // Sort left partition: use a thread if it's large
    if (i - left > 1000) {
        t1 = std::thread(parallel_quicksort, std::ref(arr), left, i - 1);
    } else {
        parallel_quicksort(arr, left, i - 1);
    }

    // Sort right partition: use a thread if it's large
    if (right - i > 1000) {
        t2 = std::thread(parallel_quicksort, std::ref(arr), i + 1, right);
    } else {
        parallel_quicksort(arr, i + 1, right);
    }

    // Join threads if they were created
    if (t1.joinable()) t1.join();
    if (t2.joinable()) t2.join();
}

int main() {
    // Example data
    std::vector<int> data = {7, 2, 6, 3, 9, 1, 5, 4, 8};

    // Call parallel quicksort on the full range
    parallel_quicksort(data, 0, data.size() - 1);

    // Print sorted result
    for (int x : data) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
