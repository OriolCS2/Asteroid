#include "Globals.h"
#include "Application.h"
#include "JSONfilepack.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL.h"
#include "ModuleUI.h"

ModuleWindow::ModuleWindow(bool start_enabled) : Module(start_enabled)
{
	name.assign("Window");
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_E("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		
		window = SDL_CreateWindow("Profile TFG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 700, flags);
		if(window == NULL)
		{
			LOG_E("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
	}
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}