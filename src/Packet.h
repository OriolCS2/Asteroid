#pragma once

#define Kilobytes(x) (1024L * x)
#define DEFAULT_PACKET_SIZE Kilobytes(4)

typedef unsigned int uint;

#include <string>

class Packet {
public:
	Packet(size_t size = DEFAULT_PACKET_SIZE);
	Packet(const std::string& d);
	Packet(char* d, size_t size);
	~Packet();

	size_t GetCapacity() const;
	size_t GetSize() const;
	char* GetBufferPtr() const;

	void Read(void* outData, size_t bytes) const;

	template< typename T >
	void Read(T& outData) const;

	void ReadString(std::string& inString) const;
	void Read(char* inString);

	template< typename T >
	const Packet& operator>>(T& data) const;

private:

	char* buffer = nullptr;
	mutable char* cursor = nullptr;
	size_t capacity = 0U;
	mutable size_t size = 0U;
};

template<typename T>
inline void Packet::Read(T& outData) const
{
	static_assert(
		std::is_arithmetic< T >::value ||
		std::is_enum< T >::value,
		"Generic Read only supports primitive data types");

	Read(&outData, sizeof(outData));
}

template<typename T>
inline const Packet& Packet::operator>>(T& data) const
{
	Read(data);
	return *this;
}
