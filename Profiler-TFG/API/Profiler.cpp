#include "Profiler.h"
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <thread>
#include <filesystem>
#include <iostream>

#include <time.h>

#pragma comment(lib, "ws2_32.lib")

#define PROTOCOL_ID 123456789
#define PACKET_SIZE 4000

enum class DataType {
	FUNCTION_BEGIN = 0,
	FUNCTION_END = 1,
	FRAME_END = 2
};

#define SetEnum(val) SetInt((int)val)

class Packet {
public:
	Packet()
	{
		memset(data, '\0', PACKET_SIZE);
		cursor = data;
		SetInt(PROTOCOL_ID);
	}

	const char* GetData()
	{
		return data;
	}

	void SetDouble(double d)
	{
		size_t size = sizeof(double);
		memcpy(cursor, &d, size);
		cursor += size;
	}

	void SetInt(int d)
	{
		size_t size = sizeof(int);
		memcpy(cursor, &d, size);
		cursor += size;
	}

	void SetChar(char d)
	{
		size_t size = sizeof(char);
		memcpy(cursor, &d, size);
		cursor += size;
	}

	void SetString(const std::string& str)
	{
		SetInt(str.size());
		
		for (auto& character : str) {
			SetChar(character);
		}
	}

private:
	char data[PACKET_SIZE];
	char* cursor = nullptr;
};

bool isConnected = false;
SOCKET client = NULL;
sockaddr_in bindAddr;
Packet* packet = nullptr;

bool HasSocketInfo(SOCKET socket)
{
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(socket, &readfds);

	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	select(0, &readfds, nullptr, nullptr, &timeout);
	return FD_ISSET(socket, &readfds);
}

ProfilerFrameData::ProfilerFrameData()
{
	if (isConnected || (isConnected = connect(client, (const sockaddr*)&bindAddr, sizeof(bindAddr)) != SOCKET_ERROR)) {
		if (HasSocketInfo(client)) {
			closesocket(client);
			client = socket(AF_INET, SOCK_STREAM, 0);
			isConnected = false;
		}
		else {
			packet = new Packet();
			clock.Start();
		}
	}
}

ProfilerFrameData::~ProfilerFrameData()
{
	if (isConnected) {
		double ms = clock.ReadMs();

		packet->SetEnum(DataType::FRAME_END);
		packet->SetDouble(ms);

		send(client, packet->GetData(), PACKET_SIZE, 0);

		delete packet;
		packet = nullptr;
	}
}

void ProfilerInit()
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		// ERROR
	}
	else {
		client = socket(AF_INET, SOCK_STREAM, 0);

		bindAddr.sin_family = AF_INET;
		bindAddr.sin_port = htons(8000);
		const char* remoteAddrStr = "127.0.0.1";
		inet_pton(AF_INET, remoteAddrStr, &bindAddr.sin_addr);
	}
}

void ProfilerCleanup()
{
	if (client != NULL) {
		closesocket(client);
	}

	if (packet != nullptr) {
		delete packet;
		packet = nullptr;
	}

	if (WSACleanup() != NO_ERROR) {
		// ERROR
	}
}

ProfilerFunctionData::ProfilerFunctionData(const char* functionName, const char* fileName, int line)
{
	if (isConnected) {
		std::string file = std::filesystem::path(fileName).stem().string();
		std::string function = functionName;

		packet->SetEnum(DataType::FUNCTION_BEGIN);
		packet->SetString(file);
		packet->SetString(function);
		packet->SetInt(line);

		clock.Start();
	}
}

ProfilerFunctionData::~ProfilerFunctionData()
{
	if (isConnected) {
		double ms = clock.ReadMs();

		packet->SetEnum(DataType::FUNCTION_END);
		packet->SetDouble(ms);
	}
}
