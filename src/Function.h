#pragma once

#include <list>
#include <string>
#include <vector>

enum class AsteroidColor;

class Function {
public:
	Function(const std::string& name, const std::string& file, int line, AsteroidColor color);
	Function() {}
	~Function();

	double GetSelfMs();
	double GetChildMs();

private:

	void CalculateChildMs();

public:

	AsteroidColor color;
	int nameIndex;
	int fileIndex;
	int line = 0;
	double ms = 0;
	std::list<Function*> functions;

private:

	double sub_functions_ms = 0;
};

struct CombinedFunction 
{
	int nameIndex;
	int fileIndex;
	int line = 0;
	double totalMS = 0;
	double maxCallMS = 0;
	int count = 0;
	double averageMS = 0;
};