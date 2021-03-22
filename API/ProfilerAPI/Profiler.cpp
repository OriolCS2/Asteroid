#include "Profiler.h"
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <thread>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define PROTOCOL_ID 123456789.0
#define PACKET_SIZE 4000

class Packet {
public:
	Packet()
	{
		memset(data, '\0', PACKET_SIZE);
		cursor = data;
		SetDouble(PROTOCOL_ID);
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

private:
	char data[PACKET_SIZE];
	char* cursor = nullptr;
};

bool isConnected = false;
SOCKET client = NULL;
sockaddr_in bindAddr;
Packet* packet = nullptr;


ProfilerFrameData::ProfilerFrameData()
{
	if (isConnected || (isConnected = connect(client, (const sockaddr*)&bindAddr, sizeof(bindAddr)) != SOCKET_ERROR)) {
		// TODO: check if recv data from profiler app to stop connection

		packet = new Packet();
		clock.Start();
	}
}

ProfilerFrameData::~ProfilerFrameData()
{
	if (isConnected) {
		packet->SetDouble(clock.ReadMs());
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
	if (WSACleanup() != NO_ERROR) {
		// ERROR
	}
}
