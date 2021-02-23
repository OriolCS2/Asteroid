#include "Frame.h"

Frame::Frame()
{
}

Frame::~Frame()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		delete* item;
	}
}

void Frame::GenerateFunctionsData()
{
	// TODO: passar per totes les funcions i ajuntar la info de totes
}

const std::list<Function*>& Frame::GetFunctionsData()
{
	if (all_functions_data.empty()) {
		GenerateFunctionsData();
	}
	return all_functions_data;
}
