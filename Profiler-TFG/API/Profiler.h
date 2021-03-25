#ifndef PROFILER
#define PROFILER

#include <chrono>

#define PROFILER_INIT() ProfilerInit();

#define PROFILER_CLEANUP() ProfilerCleanup();

#define PROFILER_START_FRAME() ProfilerFrameData profilerFrameData;

#define PROFILER_FUNCTION() ProfilerFunctionData profilerFunctionData(__FUNCTION__, __FILE__, __LINE__);

void ProfilerInit();
void ProfilerCleanup();

class Clock {
public:
	Clock() {}

	void Start()
	{
		start = std::chrono::steady_clock::now();
	}

	float ReadMs()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
	}

private:

	std::chrono::steady_clock::time_point start;
};

class ProfilerFrameData {
public:
	ProfilerFrameData();
	~ProfilerFrameData();

private:

	Clock clock;
};

class ProfilerFunctionData {
public:
	ProfilerFunctionData(const char* functionName, const char* fileName, int line);
	~ProfilerFunctionData();

private:

	Clock clock;
};

#endif
