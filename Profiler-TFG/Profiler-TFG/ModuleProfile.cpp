#include "ModuleProfile.h"
#include "Application.h"
#include "ModuleUI.h"
#include "Function.h"
#include "Frame.h"

#pragma comment(lib, "ws2_32.lib")

#define SOCKET_MAX_BUFFER 1048576

enum class DataType {
	FUNCTION_BEGIN = 0,
	FUNCTION_END = 1,
	FRAME_END = 2
};
// TOD0: borrar mmgr
ModuleProfile::ModuleProfile(bool start_enabled) : Module(start_enabled)
{
}

ModuleProfile::~ModuleProfile()
{
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

void ModuleProfile::LookForClients()
{
	if (HasSocketInfo(server)) {
		struct sockaddr_in clientAddr;
		int clientSize = sizeof(clientAddr);
		client = accept(server, (sockaddr*)&clientAddr, &clientSize);
		if (client != INVALID_SOCKET && client != SOCKET_ERROR) {
			state = ProfileState::WAITING_INFO; // TODO: fer que en el connecting surti el simbol de la rodona girant que tinc el loading de imgui widget al trello
		}
	}
}

void ModuleProfile::RecieveClientData()
{
	while (HasSocketInfo(client)) {
		char* data = new char[SOCKET_MAX_BUFFER];
		int bytes = recv(client, data, SOCKET_MAX_BUFFER, 0);

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
			// TODO: he de fer que si arriba un paquet amb el value > SOCKET_MAX_BUFFER, la proxima vegada que llegeixi que llegeixi value - SOCKET_MAX_BUFFER i faci append al que havia llegit abans
			memcpy(&value, (data + sizeof(int)), sizeof(int));
			delete[] data;

			//Packet packet = Packet(value);
			//bytes = recv(client, packet.GetBufferPtr(), packet.GetCapacity(), 0);
			//
			//if (bytes == value - SOCKET_MAX_BUFFER) {
			//	CreateData(packet);
			//}
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
