#pragma once

#include <list>

class Function;

class Frame {
public:
	Frame();
	~Frame();

	const std::list<Function*>& GetFunctionsData();

private:

	void GenerateFunctionsData();

public:

	std::list<Function*> functions;
	double ms = 0;

private:

	std::list<Function*> all_functions_data;
};
