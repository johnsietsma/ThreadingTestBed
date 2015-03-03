#include <future>
#include <iostream>
#include <thread>


// Get the answer to a time consuming problem...
int GetAnswer()
{
	for (int i = 0; i < 5; i++) {
		std::cout << "Thinking... " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	return 42;
}

void RunThreadingPromises()
{
	// Ask to the the answer sometime on the future
	std::future<int> loadedThing = std::async( GetAnswer );

	// Do ther important stuff
	for (int i = 0; i < 5; i++) {
		std::cout << "Doing stuff... " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	// Wait till the answer arrives.
	std::cout << "Answer - " << loadedThing.get() << std::endl;
}