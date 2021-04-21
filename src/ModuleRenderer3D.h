#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL_video.h"
#include "glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	SDL_GLContext context;
};