#include <assert.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>


// ---- Globals ----
static std::mutex g_CountQueueMutex;
static std::condition_variable g_IsEmptyCondition;
static std::condition_variable g_IsFullCondition;
static int g_Counter;

#define FULL_SIZE 10


// Increment the counter until it is "full"
void FillUp()
{
	std::unique_lock<std::mutex> l(g_CountQueueMutex); // Create a lock to be used by the condition
	g_IsEmptyCondition.wait(l); // Wait till the counter is "empty"

	// Fill up the counter
	for (int i = 0; i < FULL_SIZE; i++) {
		g_Counter++;
		std::cout << "Inc: " << g_Counter << std::endl;
	}

	// Signal one thread that we're "full".
	g_IsFullCondition.notify_one();
}

// Decrement the counter until it is "empty"
void Empty()
{
	std::unique_lock<std::mutex> l(g_CountQueueMutex); // Create a lock to be used by the condition
	g_IsFullCondition.wait(l); // Wait till the counter is "full"

	// Deplete the counter
	for (int i = 0; i < FULL_SIZE; i++) {
		--g_Counter;
		std::cout << "Dec: " << g_Counter << std::endl;
	}

	// Signal one thread that we're empty
	g_IsEmptyCondition.notify_one();
}


void RunThreadingConditions() {

	// Create 4 threads for filling and emptying
	std::thread incrementThread1(FillUp);
	std::thread incrementThread2(FillUp);
	std::thread decrementThread1(Empty);
	std::thread decrementThread2(Empty);

	// Tell one thread that we're empty
	g_IsEmptyCondition.notify_one();

	// Wait till we're done
	incrementThread1.join();
	incrementThread2.join();
	decrementThread1.join();
	decrementThread2.join();

	// Check the result
	std::cout << "Counter: " << g_Counter << std::endl;
	assert(g_Counter == 0);
}
