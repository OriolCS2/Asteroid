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

unsigned __int32 Function::GetSelfMs()
{
	if (functions.empty()) {
		return ms;
	}

	if (sub_functions_ms == 0) {
		CalculateChildMs();
	}
	
	return ms - sub_functions_ms;
}

unsigned __int32 Function::GetChildMs()
{
	return sub_functions_ms;
}

void Function::CalculateChildMs()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		sub_functions_ms += (*item)->ms;
	}
}
