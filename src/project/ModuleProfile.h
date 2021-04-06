#pragma once

#include "Module.h"
#include <list>

#include "Packet.h"

#define PROTOCOL_ID 123456789

enum class ProfileState {
	NONE,
	CONNECTING,
	WAITING_INFO,
	INFO
};

class Frame;
class Function;

class ModuleProfile : public Module
{
public:

	ModuleProfile(bool start_enabled = true);
	virtual ~ModuleProfile();

	bool Start();
	update_status Update(float dt);

	bool CleanUp();

	void ConnectClient();
	void DisconnectClient();
	void ResetInfo();

private:

	void LookForClients();
	void RecieveClientData();
	void CreateData(const Packet& data);

	bool HasSocketInfo(SOCKET s);
	
	void ClearFrames();

	void ReadFunctionData(const Packet& data, std::list<Function*>& toAdd);

public:
	std::list<char*> framesData;
	std::list<Frame*> frames;
	ProfileState state = ProfileState::NONE;

private:

	SOCKET server;
	SOCKET client;
};

