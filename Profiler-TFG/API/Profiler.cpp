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
#define PACKET_SIZE 32768

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
		data = new char[maxSize];
		memset(data, '\0', maxSize);
		cursor = data;
		SetInt(PROTOCOL_ID);
		cursor += sizeof(int);
		currentSize += sizeof(int);
	}

	~Packet()
	{
		delete[] data;
	}

	const char* GetData()
	{
		return data;
	}

	void SetDouble(double d)
	{
		size_t size = sizeof(double);
		UpdateSize(size);
		memcpy(cursor, &d, size);
		cursor += size;
	}

	void SetInt(int d)
	{
		size_t size = sizeof(int);
		UpdateSize(size);
		memcpy(cursor, &d, size);
		cursor += size;
	}

	void SetChar(const char* d, size_t s)
	{
		UpdateSize(s);
		memcpy(cursor, d, s);
		cursor += s;
	}

	void SetString(const std::string& str)
	{
		SetInt(str.size());
		SetChar(str.data(), str.size());
	}

	size_t GetSize() const
	{
		return currentSize;
	}

	void WriteCurrentSize()
	{
		memcpy(data + sizeof(int), &currentSize, sizeof(int));
	}

private:

	void UpdateSize(size_t size)
	{
		currentSize += size;
		while (currentSize >= maxSize) {
			size_t newSize = maxSize + PACKET_SIZE;
			char* moreData = new char[newSize];
			memcpy(moreData, data, maxSize);
			maxSize = newSize;
			delete[] data;
			data = moreData;
			cursor = data;
			cursor += (currentSize - size);
		}
	}

private:
	char* data = nullptr;
	char* cursor = nullptr;
	size_t currentSize = 0U;
	size_t maxSize = PACKET_SIZE;
};

bool isConnected = false;
SOCKET client = NULL;
sockaddr_in bindAddr;
Packet* packet = nullptr;
std::thread connectionThread;
std::atomic_bool threadSucced;
std::atomic_bool threadDead;

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

bool CheckThread()
{
	bool ret = threadSucced.load();
	if (ret) {
		threadDead.store(true);
		connectionThread.join();
	}
	return ret;
}

void TryConnection()
{
	while (!threadDead.load()) {
		if (connect(client, (const sockaddr*)&bindAddr, sizeof(bindAddr)) != SOCKET_ERROR) {
			threadSucced.store(true);
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void CreateClientSocket()
{
	isConnected = false;
	threadSucced.store(false);
	threadDead.store(false);
	if (client != NULL) {
		closesocket(client);
	}
	client = socket(AF_INET, SOCK_STREAM, 0);
	connectionThread = std::thread(TryConnection);
}

ProfilerFrameData::ProfilerFrameData()
{
	if (isConnected || (isConnected = CheckThread())) {
		if (HasSocketInfo(client)) {
			CreateClientSocket();
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

		packet->WriteCurrentSize();

		send(client, packet->GetData(), packet->GetSize(), 0);

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
		CreateClientSocket();

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

	if (!threadSucced.load()) {
		threadDead.store(true);
		connectionThread.join();
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
