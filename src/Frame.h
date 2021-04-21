#pragma once

#include <list>

class Function;
struct CombinedFunction;

class Frame {
public:
	Frame();
	~Frame();

	const std::list<CombinedFunction*>& GetFunctionsData();

private:

	void GenerateFunctionsData();

public:

	std::list<Function*> functions;
	double ms = 0;

private:

	std::list<CombinedFunction*> all_functions_data;
};
