#include "BinarySave.h"
#include <Windows.h>
#include "ModuleProfile.h"
#include "Application.h"

BinarySave::BinarySave()
{
	data = (char*)VirtualAlloc(NULL, maxSize, MEM_RESERVE, PAGE_READWRITE);
	VirtualAlloc(data, maxSize, MEM_COMMIT, PAGE_READWRITE);
	memset(data, '\0', maxSize);
	cursor = data;
}

BinarySave::~BinarySave()
{
	VirtualFree(data, 0, MEM_RELEASE);
}

void BinarySave::SetString(const std::string& str)
{
	SetNumber(str.size());
	if (UpdateSize(str.size())) {
		memcpy(cursor, str.data(), str.size());
		cursor += str.size();
	}
}

void BinarySave::Save(const std::string& path)
{
	FILE* pFile;
	pFile = fopen(path.data(), "w+b");
	fwrite(data, 1, currentSize, pFile);
	fclose(pFile);
}

#define MAX_MEMPCY 50000000

bool BinarySave::UpdateSize(size_t size)
{
	currentSize += size;
	while (currentSize >= maxSize) {
		size_t newSize = maxSize + BINARY_SAVE_SIZE;
		char* moreData = (char*)VirtualAlloc(NULL, newSize, MEM_RESERVE, PAGE_READWRITE);
		if (moreData != nullptr) {
			VirtualAlloc(moreData, newSize, MEM_COMMIT, PAGE_READWRITE);
			size_t toCopy = min(MAX_MEMPCY, maxSize);
			char* c = data;
			size_t copied = 0;
			while (copied < maxSize) {
				memcpy(moreData + copied, c, toCopy);
				copied += toCopy;
				c += copied;
				toCopy = min(MAX_MEMPCY, (maxSize - copied));
			}
			VirtualFree(data, 0, MEM_RELEASE);
			maxSize = newSize;
			data = moreData;
			cursor = data;
			cursor += (currentSize - size);
		}
		else {
			App->profile->stopSave = true;
			return false;
		}
	}
	return true;
}
