#include <assert.h>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#define NUM_THREADS 5 // The total number of threads spawned
#define NUMBER_OF_INCREMENTS 10000 // The total number of increments per thread

static std::mutex g_IncrementMutex;


/**
 * A simple counter class demonstrating basic thread safety.
 */
class Counter
{
public:
	int GetCount() const { 
		  return count;
		  //return atomicCount;
	}

	void Increment() {
		SimpleIncrement();
		//MutexIncrement();
		//LockGuardIncrement();
		//AtomicIncrement();
	}

private:
	// Simple increment, not thread safe
	void SimpleIncrement() {
		++count;
	}

	// Thread safe increment using a mutex
	void MutexIncrement() {
		g_IncrementMutex.lock();
		++count;
		g_IncrementMutex.unlock();
	}


	// Thread safe increment using a lock guard
	void LockGuardIncrement() {
		std::lock_guard<std::mutex> lock(g_IncrementMutex);
		++count;
	}

	// Thread safe increment using std::atomic
	void AtomicIncrement() {
		atomicCount++;
	}

	int count = 0;
	std::atomic<int> atomicCount = 0;
};


// Increment the counter a set number of times.
// This will be executed by each thread concurrently.
void Increment(Counter* counter)
{
	for (int i = 0; i < NUMBER_OF_INCREMENTS; i++) {
		counter->Increment();
	}
}


void RunThreadingLocks() {
	Counter counter;

	// Start a set number of threads
	std::thread threads[NUM_THREADS];
	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads[i] = std::thread(Increment, &counter);
	}

	// Wait for them all to finish
	for (std::thread& t : threads)
	{
		t.join();
	}

	// Check the result
	std::cout << "Counter: " << counter.GetCount() << std::endl;
	assert(counter.GetCount() == NUM_THREADS*NUMBER_OF_INCREMENTS);
}

