#pragma once

#include <list>

class Function;

class Frame {
public:
	Frame();
	~Frame();

public:

	std::list<Function*> functions;
	unsigned __int32 ms = 0;
};
