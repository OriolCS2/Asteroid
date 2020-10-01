#ifndef __ModuleWProfile_H__
#define __ModuleWProfile_H__

#include "Module.h"
#include <list>

class Frame;

class ModuleProfile : public Module
{
public:

	ModuleProfile(bool start_enabled = true);
	virtual ~ModuleProfile();

	bool Start();

	bool CleanUp();

public:
	std::list<Frame*> frames;
};

#endif
