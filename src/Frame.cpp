#include "Frame.h"
#include <stack>
#include "Function.h"

Frame::Frame()
{
}

Frame::~Frame()
{
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		delete* item;
	}

	for (auto item = all_functions_data.begin(); item != all_functions_data.end(); ++item) {
		delete* item;
	}
}

void Frame::GenerateFunctionsData()
{
	std::stack<Function*> functs;
	for (auto item = functions.begin(); item != functions.end(); ++item) {
		functs.push(*item);
	}

	while (!functs.empty()) {
		Function* function = functs.top();
		functs.pop();

		bool canAdd = true;
		for (auto item = all_functions_data.begin(); item != all_functions_data.end(); ++item) {
			if ((*item)->line == function->line && (*item)->nameIndex == function->nameIndex && (*item)->fileIndex == function->fileIndex) {
				canAdd = false;
				++(*item)->count;
				(*item)->totalMS += function->ms;
				if ((*item)->maxCallMS < function->ms) {
					(*item)->maxCallMS = function->ms;
				}
			}
		}

		if (canAdd) {
			CombinedFunction* f = new CombinedFunction();
			f->count = 1;
			f->fileIndex = function->fileIndex;
			f->line = function->line;
			f->maxCallMS = function->ms;
			f->nameIndex = function->nameIndex;
			f->totalMS = function->ms;
			all_functions_data.push_back(f);
		}

		for (auto item = function->functions.begin(); item != function->functions.end(); ++item) {
			functs.push(*item);
		}
	}

	for (auto item = all_functions_data.begin(); item != all_functions_data.end(); ++item) {
		(*item)->averageMS = (*item)->totalMS / (*item)->count;
	}
}

const std::list<CombinedFunction*>& Frame::GetFunctionsData()
{
	if (all_functions_data.empty()) {
		GenerateFunctionsData();
	}
	return all_functions_data;
}
