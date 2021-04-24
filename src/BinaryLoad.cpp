#include "BinaryLoad.h"
#include <fstream>
#include <Windows.h>

BinaryLoad::BinaryLoad(const std::string& path)
{
	std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	data = (char*)VirtualAlloc(NULL, size, MEM_RESERVE, PAGE_READWRITE);
	VirtualAlloc(data, size, MEM_COMMIT, PAGE_READWRITE);

	file.read(data, size);
	cursor = data;
}

BinaryLoad::~BinaryLoad()
{
	VirtualFree(data, 0, MEM_RELEASE);
}

std::string BinaryLoad::GetString()
{
	size_t size = GetNumber<size_t>();
	std::string s;
	s.reserve(size);
	
	char* d = new char[size];

	memcpy(d, cursor, size);
	cursor += size;
	s = d;

	delete[] d;

	return s;
}
