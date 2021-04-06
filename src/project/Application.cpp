#include "Application.h"

#include "Time.h"
#include "JSONfilepack.h"
#include "JSONfilepack.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleProfile.h"
#include "ModuleUI.h"
#include "ModuleRenderer3D.h"

Application::Application()
{
	time_since_start.Start();

	window = new ModuleWindow();
	input = new ModuleInput();
	ui = new ModuleUI();
	renderer = new ModuleRenderer3D();
	profile = new ModuleProfile();

	AddModule(window);
	AddModule(input);
	AddModule(profile);
	AddModule(ui);
	AddModule(renderer);
}

Application::~Application()
{
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend())
	{
		delete(*item);
		++item;
	}

	list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		++item;
	}

	// After all Init calls we call Start() in all modules
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		++item;
	}

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	++last_sec_frame_count;
	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_count = 0;
		last_sec_frame_time.Start();
	}

	unsigned __int32 last_frame_ms = frame_time.Read();

	if (is_capping_fps && max_frame_ms > 0 && last_frame_ms < max_frame_ms) {
		SDL_Delay(max_frame_ms - last_frame_ms);
	}
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		++item;
	}
	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		++item;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		++item;
	}

	if (quit) {
		ret = UPDATE_STOP;
	}

	FinishUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		++item;
	}
	return ret;
}

void Application::QuitApp()
{
	quit = true;
}

bool Application::IsQuiting() const
{
	return quit;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}