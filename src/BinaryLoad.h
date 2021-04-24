#pragma once

#include <string>

class BinaryLoad {
public:
	BinaryLoad(const std::string& path);
	~BinaryLoad();

	template<typename T>
	T GetNumber();

	std::string GetString();

private:
	char* data = nullptr;
	char* cursor = nullptr;
};

template<typename T>
inline T BinaryLoad::GetNumber()
{
	T item;
	size_t size = sizeof(T);
	memcpy(&item, cursor, size);
	cursor += size;
	return item;
}
