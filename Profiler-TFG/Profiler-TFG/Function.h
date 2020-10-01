#pragma once

#pragma once

#include <list>
#include <string>

class Function {
public:
	Function(const std::string& name, const std::string& file, int line);
	~Function();

public:

	std::string name;
	std::string file;
	int line = 0;
	unsigned __int32 ms = 0;
	std::list<Function*> functions;
};
