#include "Function.h"

Function::Function(const std::string& name, const std::string& file, int line)
	: name(name), file(file), line(line)
{
}

Function::~Function()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		delete* item;
	}
}
