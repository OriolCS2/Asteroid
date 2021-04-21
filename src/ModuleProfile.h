#pragma once

#include "Module.h"
#include <list>
#include <vector>
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

class JSONArraypack;
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

	void SaveCurrentDataToFile(const std::string& file);
	void LoadFile(const std::string& file);

	int GetFileStringIndex(const std::string& file);
	int GetNameStringIndex(const std::string& name);

private:

	void SaveFunction(Function* function, JSONArraypack* to_save);
	void LoadFunction(Function* function, JSONArraypack* to_load);

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

	std::vector<std::string> functionNames;
	std::vector<std::string> fileNames;

private:

	bool exitThreadFlag = false;
	std::condition_variable event;
	std::mutex mtx;
	std::thread parseThread;

	SOCKET server;
	SOCKET client;
};

