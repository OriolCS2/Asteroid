#pragma once

#include "Module.h"
#include <list>

#include "Packet.h"
#include <thread>
#include <mutex>

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

	void ParseData();

	void LookForClients();
	void RecieveClientData();
	void CreateData(const Packet& data);

	bool HasSocketInfo(SOCKET s);
	
	void ClearFrames();

	void ReadFunctionData(const Packet& data, std::list<Function*>& toAdd);

public:
	int framesCount = 0;
	std::list<Packet*> framesData;
	std::list<Frame*> frames;
	ProfileState state = ProfileState::NONE;

private:

	bool exitThreadFlag = false;
	std::condition_variable event;
	std::mutex mtx;
	std::thread parseThread;

	SOCKET server;
	SOCKET client;
};

