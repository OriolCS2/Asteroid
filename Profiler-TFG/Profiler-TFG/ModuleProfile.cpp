#include "ModuleProfile.h"
#include "Application.h"
#include "ModuleUI.h"
#include "Function.h"
#include "Frame.h"

#pragma comment(lib, "ws2_32.lib")


ModuleProfile::ModuleProfile(bool start_enabled) : Module(start_enabled)
{
}

ModuleProfile::~ModuleProfile()
{
}

bool ModuleProfile::Start()
{
	auto AddFunction = [this](Frame* frame, const char* name, const char* file, int line, int time) {
		Function* f = new Function(name, file, line);
		f->ms = time;
		frame->functions.push_back(f);
	};

	Frame* frame = new Frame();
	frame->ms = 38;
	frames.push_back(frame);

	AddFunction(frame, "PreUpdate", "App", 10, 6);
	AddFunction(frame, "Update", "App", 16, 20);
	frame->functions.back()->functions.push_back(new Function("Render", "ModuleRenderer", 20));
	frame->functions.back()->functions.back()->ms = 14;
	AddFunction(frame, "PostUpdate", "App", 22, 12);

	frame = new Frame();
	frame->ms = 47;
	frames.push_back(frame);
	AddFunction(frame, "PreUpdate", "App", 10, 10);
	AddFunction(frame, "Update", "App", 16, 25);
	frame->functions.back()->functions.push_back(new Function("Render", "ModuleRenderer", 20));
	frame->functions.back()->functions.back()->ms = 14;
	AddFunction(frame, "PostUpdate", "App", 22, 12);

	frame = new Frame();
	frame->ms = 9;
	frames.push_back(frame);
	AddFunction(frame, "PreUpdate", "App", 10, 3);
	AddFunction(frame, "Update", "App", 16, 4);
	frame->functions.back()->functions.push_back(new Function("Render", "ModuleRenderer", 3));
	frame->functions.back()->functions.back()->ms = 3.8;
	AddFunction(frame, "PostUpdate", "App", 22, 2);

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
	// TODO: send something to client to close client
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
		Packet packet;
		int bytes = recv(client, packet.GetBufferPtr(), packet.GetCapacity(), 0);

		if (bytes == SOCKET_ERROR || bytes == ECONNRESET || bytes == 0) {
			DisconnectClient();
			break;
		}
		else {
			CreateData(packet);
		}
	}
}

void ModuleProfile::CreateData(const Packet& data)
{
	int protoId;
	data >> protoId;
	if (protoId != PROTOCOL_ID) return;

	LOG("Works");
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
