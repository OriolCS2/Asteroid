#include "ModuleProfile.h"
#include "Application.h"
#include "ModuleUI.h"
#include "JSONfilepack.h"
#include "Function.h"
#include "Frame.h"
#include <stack>

#pragma comment(lib, "ws2_32.lib")

#define SOCKET_MAX_BUFFER 1048576

enum class DataType {
	FUNCTION_BEGIN = 1,
	FUNCTION_END = 2,
	FRAME_END = 3
};

ModuleProfile::ModuleProfile(bool start_enabled) : Module(start_enabled)
{
}

ModuleProfile::~ModuleProfile()
{
	if (state == ProfileState::WAITING_INFO) {
		DisconnectClient();
	}

	for (auto item = framesData.begin(); item != framesData.end(); ++item) {
		delete* item;
	}

	ClearFrames();
}

bool ModuleProfile::Start()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		LOG("Error sockets api");
	}

	return true;
}

update_status ModuleProfile::Update(float dt)
{
	switch (state) {
	case ProfileState::CONNECTING: {
		LookForClients();
		break; }
	case ProfileState::WAITING_INFO: {
		RecieveClientData();
		break; }
	}

	return UPDATE_CONTINUE;
}

bool ModuleProfile::CleanUp()
{
	WSACleanup();
	ClearFrames();
	return true;
}

void ModuleProfile::ConnectClient()
{
	state = ProfileState::CONNECTING;

	server = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET; // IPv4
	bindAddr.sin_port = htons(8000); // Port
	bindAddr.sin_addr.S_un.S_addr = INADDR_ANY; // Any local IP address

	bind(server, (const struct sockaddr*)&bindAddr, sizeof(bindAddr));

	if (listen(server, 1) == SOCKET_ERROR) {
		LOG("Socket fail listening!");
	}
}

void ModuleProfile::DisconnectClient()
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		exitThreadFlag = true;
		event.notify_one();
	}
	parseThread.join();
	state = ProfileState::INFO;

	closesocket(server);
	closesocket(client);
}

void ModuleProfile::ResetInfo()
{
	if (state == ProfileState::INFO) {
		ClearFrames();
		App->ui->OnFrameDeselected();
		state = ProfileState::NONE;
	}
}

void ModuleProfile::SaveCurrentDataToFile(const std::string& file)
{
	JSONfilepack* json = JSONparser::CreateJSON(file.data());

	json->StartSave();

	JSONArraypack* framesArray = json->InitNewArray("Frames");

	for (auto item = frames.begin(); item != frames.end(); ++item) {
		framesArray->SetAnotherNode();
		framesArray->SetNumber("ms", (*item)->ms);

		if (!(*item)->functions.empty()) {
			JSONArraypack* functionsArray = framesArray->InitNewArray("Functions");
			for (auto it = (*item)->functions.begin(); it != (*item)->functions.end(); ++it) {
				functionsArray->SetAnotherNode();
				SaveFunction(*it, functionsArray);
			}
		}
	}

	json->FinishSave();

	JSONparser::FreeJSON(json);
}

void ModuleProfile::LoadFile(const std::string& file)
{
	ClearFrames();

	JSONfilepack* json = JSONparser::GetJSON(file.data());

	JSONArraypack* framesArray = json->GetArray("Frames");
	if (framesArray != nullptr) {
		for (int i = 0; i < framesArray->GetArraySize(); ++i) {
			Frame* frame = new Frame();
			frames.push_back(frame);

			frame->ms = framesArray->GetNumber("ms");
			JSONArraypack* functionsArray = framesArray->GetArray("Functions");
			if (functionsArray != nullptr) {
				for (int j = 0; j < functionsArray->GetArraySize(); ++j) {
					Function* function = new Function();
					frame->functions.push_back(function);

					LoadFunction(function, functionsArray);

					functionsArray->GetAnotherNode();
				}
			}


			framesArray->GetAnotherNode();
		}
	}

	JSONparser::FreeJSON(json);

	state = ProfileState::INFO;
}

void ModuleProfile::SaveFunction(Function* function, JSONArraypack* to_save)
{
	to_save->SetNumber("ms", function->ms);
	to_save->SetNumber("line", function->line);
	to_save->SetString("file", function->file);
	to_save->SetString("name", function->name);

	JSONArraypack* functionsArray = to_save->InitNewArray("Functions");
	for (auto it = function->functions.begin(); it != function->functions.end(); ++it) {
		functionsArray->SetAnotherNode();
		SaveFunction(*it, functionsArray);
	}
}

void ModuleProfile::LoadFunction(Function* function, JSONArraypack* to_load)
{
	function->ms = to_load->GetNumber("ms");
	function->line = to_load->GetNumber("line");
	function->file = to_load->GetString("file");
	function->name = to_load->GetString("name");

	JSONArraypack* functionsArray = to_load->GetArray("Functions");
	if (functionsArray != nullptr) {
		for (int j = 0; j < functionsArray->GetArraySize(); ++j) {
			Function* f = new Function();
			function->functions.push_back(f);

			LoadFunction(f, functionsArray);

			functionsArray->GetAnotherNode();
		}
	}
}

void ModuleProfile::ParseData()
{
	while (true) {

		Packet* data = nullptr;
		{
			std::unique_lock<std::mutex> lock(mtx);
			if (exitThreadFlag) {
				exitThreadFlag = false;
				break;
			}

			if (!framesData.empty()) {
				data = framesData.front();
				framesData.pop_front();
			}
			else {
				if (exitThreadFlag) {
					exitThreadFlag = false;
					break;
				}
				event.wait(lock);
			}
		}

		if (data != nullptr) {
			CreateData(*data);
			delete data;
		}
	}
}

void ModuleProfile::LookForClients()
{
	if (HasSocketInfo(server)) {
		struct sockaddr_in clientAddr;
		int clientSize = sizeof(clientAddr);
		client = accept(server, (sockaddr*)&clientAddr, &clientSize);
		if (client != INVALID_SOCKET && client != SOCKET_ERROR) {
			parseThread = std::thread(&ModuleProfile::ParseData, this);
			framesCount = 0;
			state = ProfileState::WAITING_INFO;
		}
	}
}

void ModuleProfile::RecieveClientData()
{
	while (HasSocketInfo(client)) {
		char* data = new char[sizeof(int) * 2];
		int bytes = recv(client, data, sizeof(int) * 2, 0);

		if (bytes == SOCKET_ERROR || bytes == ECONNRESET || bytes == 0) {
			DisconnectClient();
			delete[] data;
			break;
		}
		else {
			int value;
			memcpy(&value, data, sizeof(int));

			if (value != PROTOCOL_ID) {
 				delete[] data;
				break;
			}

			memcpy(&value, (data + sizeof(int)), sizeof(int));
			value -= sizeof(int) * 2;
			delete[] data;
			char* frameData = new char[value];

			int bytesRead = 0;
			while (bytesRead != value) {
				int toRead = value - bytesRead > SOCKET_MAX_BUFFER ? SOCKET_MAX_BUFFER : value - bytesRead;
				char* cursor = frameData + bytesRead;
				bytes = recv(client, cursor, toRead, 0);

				bytesRead += toRead;
			}

			++framesCount;
			Packet* packet = new Packet(frameData, value);
			{
				std::unique_lock<std::mutex> lock(mtx);
				framesData.push_back(packet);
				event.notify_one();
			}
		}
	}
}

void ModuleProfile::CreateData(const Packet& data)
{
	Frame* frame = new Frame();

	DataType type;
	data >> type;
	while (type == DataType::FUNCTION_BEGIN) {
		ReadFunctionData(data, frame->functions);
		data >> type;
	}

	data >> frame->ms;
	
	frames.push_back(frame);
}

bool ModuleProfile::HasSocketInfo(SOCKET s)
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(s, &readfds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	select(0, &readfds, nullptr, nullptr, &timeout);
	return FD_ISSET(s, &readfds);
}

void ModuleProfile::ClearFrames()
{
	for (auto item = frames.begin(); item != frames.end(); ++item) {
		delete* item;
	}
	frames.clear();
}

void ModuleProfile::ReadFunctionData(const Packet& data, std::list<Function*>& toAdd)
{
	std::string fileName, functionName;
	int line;
	
	data.ReadString(fileName);
	data.ReadString(functionName);
	data >> line;

	Function* function = new Function(functionName, fileName, line);
	toAdd.push_back(function);

	DataType type;
	data >> type;
	while (type == DataType::FUNCTION_BEGIN) {
		ReadFunctionData(data, function->functions);
		data >> type;
	}

	data >> function->ms;
}