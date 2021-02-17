#pragma once

#pragma once

#include <list>
#include <string>

class Function {
public:
	Function(const std::string& name, const std::string& file, int line);
	~Function();

	unsigned __int32 GetSelfMs();
	unsigned __int32 GetChildMs();

private:

	void CalculateChildMs();

public:

	std::string name;
	std::string file;
	int line = 0;
	unsigned __int32 ms = 0;
	std::list<Function*> functions;

private:

	unsigned __int32 sub_functions_ms = 0;
};
