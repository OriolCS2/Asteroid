#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include "Profiler.h"

void Update() 
{
	PROFILER_FUNCTION();
	std::this_thread::sleep_for(std::chrono::milliseconds(3));
}

void main()
{
	PROFILER_INIT();

	while (true)
	{
		PROFILER_START_FRAME();
	
		

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	system("pause");

	PROFILER_CLEANUP();
}