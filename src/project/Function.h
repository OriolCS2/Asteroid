#pragma once

#pragma once

#include <list>
#include <string>

class Function {
public:
	Function(const std::string& name, const std::string& file, int line);
	Function() {}
	~Function();

	double GetSelfMs();
	double GetChildMs();

private:

	void CalculateChildMs();

public:

	std::string name;
	std::string file;
	int line = 0;
	double ms = 0;
	std::list<Function*> functions;

private:

	double sub_functions_ms = 0;
};
