#include "ModuleProfile.h"
#include "Application.h"
#include "Function.h"
#include "Frame.h"

ModuleProfile::ModuleProfile(bool start_enabled) : Module(start_enabled)
{
}

ModuleProfile::~ModuleProfile()
{
}

bool ModuleProfile::Start()
{
	auto AddFunction = [this](Frame* frame, const char* name, const char* file, int line, int time) {
		Function* f = new Function(name, file, line);
		f->ms = time;
		frame->functions.push_back(f);
	};

	Frame* frame = new Frame();
	frame->ms = 680;
	frames.push_back(frame);

	AddFunction(frame, "PreUpdate", "App", 10, 6);
	AddFunction(frame, "Update", "App", 16, 20);
	frame->functions.back()->functions.push_back(new Function("Render", "ModuleRenderer", 20));
	frame->functions.back()->functions.back()->ms = 14;
	AddFunction(frame, "PostUpdate", "App", 22, 10);

	frame = new Frame();
	frame->ms = 47;
	frames.push_back(frame);
	AddFunction(frame, "PreUpdate", "App", 10, 10);
	AddFunction(frame, "Update", "App", 16, 23);
	frame->functions.back()->functions.push_back(new Function("Render", "ModuleRenderer", 20));
	frame->functions.back()->functions.back()->ms = 14;
	AddFunction(frame, "PostUpdate", "App", 22, 12);

	return true;
}

bool ModuleProfile::CleanUp()
{
	for (auto item = frames.begin(); item != frames.end(); ++item) {
		delete* item;
	}
	frames.clear();

	return true;
}
