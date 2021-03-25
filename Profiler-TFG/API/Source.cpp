#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include <Windows.h>
#include "Profiler.h"

void Update() 
{
	PROFILER_FUNCTION();
	Sleep(3);
}

void PostUpdate()
{
	PROFILER_FUNCTION();
	Clock c;
	c.Start();
	Sleep(4);
	double d = c.ReadMs();
	printf(" %f", d);
}

void main()
{
	PROFILER_INIT();

	while (true)
	{
		PROFILER_START_FRAME();
	
		Update();
		PostUpdate();

		Sleep(10);
	}

	system("pause");

	PROFILER_CLEANUP();
}