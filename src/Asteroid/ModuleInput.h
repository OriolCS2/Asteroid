#pragma once
#include "Module.h"
#include "Globals.h"
#include "imgui/imgui.h"
#include "SDL/include/SDL_scancode.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include <map>

#define MAX_MOUSE_BUTTONS 6

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(bool start_enabled = true);
	~ModuleInput();

	bool Init();
	update_status PreUpdate(float dt);
	bool CleanUp();

	KEY_STATE GetKey(int id) const;
	KEY_STATE GetMouseButton(int id);

	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseZ() const;
	float3 GetMousePosition();
	int GetMouseXMotion() const;
	int GetMouseYMotion() const;
	float2 GetMouseMotion() const;

	float MapValue(Sint16 unmapped, Sint16 dead_zone);

private:

	float Map(float s, float a1, float a2, float b1, float b2);

	void UpdateMouseState();
	void UpdateKeyboardState();
	void UpdateSDLEvents();

private:
	KEY_STATE* keyboard;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
	int mouse_z;
	int mouse_x_motion;
	int mouse_y_motion;

public:

	bool anyKeyDown = false;
	bool anyKeyRepeat = false;
	bool anyKeyUp = false;

	bool anyMouseButtonDown = false;
	bool anyMouseButtonRepeat = false;
	bool anyMouseButtonUp = false;
};