#pragma once

#include <string>

#define BINARY_SAVE_SIZE 12000

class BinarySave {
public:
	BinarySave();
	~BinarySave();

	template<typename T>
	void SetNumber(T number);

	void SetString(const std::string& str);

	void Save(const std::string& path);

private:
	bool UpdateSize(size_t size);
private:
	char* data = nullptr;
	char* cursor = nullptr;
	size_t currentSize = 0U;
	size_t maxSize = BINARY_SAVE_SIZE;
};

template<typename T>
inline void BinarySave::SetNumber(T number)
{
	size_t size = sizeof(T);
	if (UpdateSize(size)) {
		memcpy(cursor, &number, size);
		cursor += size;
	}
}
