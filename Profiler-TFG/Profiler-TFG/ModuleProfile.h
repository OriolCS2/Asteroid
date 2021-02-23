#ifndef __ModuleWProfile_H__
#define __ModuleWProfile_H__

#include "Module.h"
#include <list>

enum class ProfileState {
	NONE,
	WAITING_INFO,
	INFO
};

class Frame;

class ModuleProfile : public Module
{
public:

	ModuleProfile(bool start_enabled = true);
	virtual ~ModuleProfile();

	bool Start();

	bool CleanUp();

	void ConnectClient();
	void DisconnectClient();
	void ResetInfo();

private:

	void ClearFrames();

public:
	std::list<Frame*> frames;
	ProfileState state = ProfileState::NONE;
};

#endif
