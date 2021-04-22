#ifndef PROFILER
#define PROFILER

#include <chrono>

#define ASTEROID_INIT() AsteroidInit();

#define ASTEROID_CLEANUP() AsteroidCleanup();

#define ASTEROID_FRAME() AsteroidFrameData profilerFrameData;

#define ASTEROID_FUNCTION() AsteroidFunctionData profilerFunctionData(__FUNCTION__, __FILE__, __LINE__, sizeof(__FUNCTION__), sizeof(__FILE__));

#define ASTEROID_FUNCTION_EX(NAME) AsteroidFunctionData profilerFunctionData(NAME, __FILE__, __LINE__, sizeof(NAME), sizeof(__FILE__));

#define ASTEROID_CATEGORY(NAME, COLOR) AsteroidFunctionData profilerFunctionData(NAME, __FILE__, __LINE__, sizeof(NAME), sizeof(__FILE__), COLOR);

void AsteroidInit();
void AsteroidCleanup();

enum class AsteroidColor {
	BLACK = 0,
	BLUE = 1,
	CYAN = 2,
	GRAY = 3,
	GREEN = 4,
	MAGENTA = 5,
	RED = 6,
	WHITE = 7,
	YELLOW = 8,
	ORANGE = 9,
	PURPLE = 10,
	NONE = -1
};

class Clock {
public:
	Clock() {}

	void Start()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	double ReadMs()
	{
		return  std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start).count();
	}

private:

	std::chrono::steady_clock::time_point start;
};

class AsteroidFrameData {
public:
	AsteroidFrameData();
	~AsteroidFrameData();

private:

	Clock clock;
};

class AsteroidFunctionData {
public:
	AsteroidFunctionData(const char* functionName, const char* fileName, int line, int functionSize, int fileSize, AsteroidColor color = AsteroidColor::NONE);
	~AsteroidFunctionData();

private:

	Clock clock;
};

#endif
