#include "Function.h"
#include "Application.h"
#include "ModuleProfile.h"
#include <filesystem>

Function::Function(const std::string& name, const std::string& file, int line, AsteroidColor color)
	: line(line), color(color)
{
	fileIndex = App->profile->GetFileStringIndex(std::filesystem::path(file).filename().string());
	nameIndex = App->profile->GetNameStringIndex(name);
}

Function::~Function()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		delete* item;
	}
}

double Function::GetSelfMs()
{
	if (functions.empty()) {
		return ms;
	}

	if (sub_functions_ms == 0) {
		CalculateChildMs();
	}
	
	return ms - sub_functions_ms;
}

double Function::GetChildMs()
{
	return sub_functions_ms;
}

void Function::CalculateChildMs()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		sub_functions_ms += (*item)->ms;
	}
}
