#include "PerfProfiler.h"
#include <iostream>
#include <future>

void function1()
{
	PROFILE_FUNCTION();
	for (int i = 0; i < 1; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		PROFILE_SCOPE("int* a = new int", "new");
		const int* a = new int;
	}
}

void function2()
{
	PROFILE_FUNCTION();
	for (int i = 0; i < 1; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		const int *a = new int;
	}
}

void ProfileTest()
{
	//PerProfiler::Instance().beginSession("main");
	auto f1 = std::async(std::launch::async, function1);
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	auto f2 = std::async(std::launch::async, function2);
	f1.get();
	f2.get();
	//PerProfiler::Instance().endSession();
}


int main()
{
	ProfileTest();

}