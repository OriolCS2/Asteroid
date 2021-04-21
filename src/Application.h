#pragma once

#include "Globals.h"
#include "Timer.h"
#include <list>
#include "j1PerfTimer.h"
#include <string>
#include <vector>

class Module;
class ModuleWindow;
class ModuleInput;
class ModuleUI;
class ModuleRenderer3D;
class ModuleProfile;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void QuitApp();
	bool IsQuiting() const;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleUI* ui = nullptr;
	ModuleRenderer3D* renderer = nullptr;
	ModuleProfile* profile = nullptr;

private:
	std::list<Module*> list_modules;
	bool quit = false;

	Timer frame_time;
	Timer last_sec_frame_time;
	unsigned __int32 last_sec_frame_count = 0;

	bool is_capping_fps = false;
	unsigned short max_frame_ms = 0;
public:
	float dt = 0.0F;
	Timer time_since_start;
};

extern Application* App;


