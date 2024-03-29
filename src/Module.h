#pragma once

#include <string>
#include "Globals.h"

class Module
{
private :
	bool enabled = true;

public:

	Module(bool start_enabled = true) { enabled = start_enabled; }

	virtual ~Module() {}

	virtual bool Init() 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt) 
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt) 
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt) 
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}
		
	std::string name;
};