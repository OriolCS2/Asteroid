#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "imgui/imgui_impl_sdl.h"
#include "ModuleRenderer3D.h"
#include "Time.h"
#include "mmgr/mmgr.h"

#define MAX_KEYS 300

ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name.assign("Input");
	
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	bool ret = true;
	SDL_Init(0);
	
	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG_E("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	UpdateKeyboardState();

	UpdateMouseState();

	UpdateSDLEvents();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KEY_STATE ModuleInput::GetKey(int id) const
{
	return keyboard[id];
}

KEY_STATE ModuleInput::GetMouseButton(int id)
{
	return mouse_buttons[id];
}

int ModuleInput::GetMouseX() const
{
	return mouse_x;
}

int ModuleInput::GetMouseY() const
{
	return mouse_y;
}

int ModuleInput::GetMouseZ() const
{
	return mouse_z;
}

int ModuleInput::GetMouseXMotion() const
{
	return mouse_x_motion;
}

int ModuleInput::GetMouseYMotion() const
{
	return mouse_y_motion;
}

float2 ModuleInput::GetMouseMotion() const
{
	return float2(mouse_x_motion, mouse_y_motion);
}

float ModuleInput::MapValue(Sint16 unmapped, Sint16 dead_zone)
{
	if (unmapped > dead_zone) {
		return Map(unmapped, dead_zone, SDL_MAX_SINT16, 0, 1.0f);
	}
	else if (unmapped < -dead_zone) {
		return Map(unmapped, -dead_zone, SDL_MIN_SINT16, 0, -1.0F);
	}
	return 0;
}

float3 ModuleInput::GetMousePosition()
{
	return float3(mouse_x, mouse_y, mouse_z);
}

float ModuleInput::Map(float s, float a1, float a2, float b1, float b2)
{
	return b1 + (s - a1) * (b2 - b1) / (a2 - a1);
}

void ModuleInput::UpdateMouseState()
{
	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_z = 0;
	anyMouseButtonDown = anyMouseButtonRepeat = anyMouseButtonUp = false;

	for (int i = 0; i < MAX_MOUSE_BUTTONS; ++i)
	{
		if (buttons & SDL_BUTTON(i))
		{
			if (mouse_buttons[i] == KEY_IDLE) {
				mouse_buttons[i] = KEY_DOWN;
				anyMouseButtonDown = true;
			}
			else {
				mouse_buttons[i] = KEY_REPEAT;
				anyMouseButtonRepeat = true;
			}
		}
		else
		{
			if (mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN) {
				mouse_buttons[i] = KEY_UP;
				anyMouseButtonUp = true;
			}
			else {
				mouse_buttons[i] = KEY_IDLE;
			}
		}
	}
}

void ModuleInput::UpdateKeyboardState()
{
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	anyKeyDown = anyKeyRepeat = anyKeyUp = false;

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1) {
			if (keyboard[i] == KEY_IDLE) {
				keyboard[i] = KEY_DOWN;
				anyKeyDown = true;
			}
			else {
				keyboard[i] = KEY_REPEAT;
				anyKeyRepeat = true;
			}
		}
		else {
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN) {
				keyboard[i] = KEY_UP;
				anyKeyUp = true;
			}
			else {
				keyboard[i] = KEY_IDLE;
			}
		}
	}
}

void ModuleInput::UpdateSDLEvents()
{
	mouse_x_motion = mouse_y_motion = 0;
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		switch (e.type)
		{
		case SDL_MOUSEWHEEL: {
			mouse_z = e.wheel.y;
			break; }
		case SDL_MOUSEMOTION: {
			mouse_x = e.motion.x;
			mouse_y = e.motion.y;

			mouse_x_motion = e.motion.xrel;
			mouse_y_motion = e.motion.yrel;
			break; }
		case SDL_WINDOWEVENT: {
			switch (e.window.event) {
			case SDL_WINDOWEVENT_RESIZED: {
				App->renderer->OnResize(e.window.data1, e.window.data2);
				break; }
			}
			break; }
		case SDL_QUIT: {
			App->QuitApp();
			break; }
		}
	}
}