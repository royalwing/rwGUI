#include "Memory.h"


size_t rw::strlen(const char* str)
{
	if (str == nullptr) return 0;
	size_t res;
	for (res = 0; str[res] != '\0'; res++);
	return res;
}

size_t rw::strlen(const wchar_t* str)
{
	if (str == nullptr) return 0;
	size_t res;
	for (res = 0; str[res] != L'\0'; res++);
	return res;
}

void rw::memcpy(void* dest, const void* src, size_t size)
{
	for (size_t pos = 0; pos < size; pos++)
		((char*)(dest))[pos] = ((char*)(src))[pos];
}

void rw::memmove(void* dest, const void* src, size_t size)
{
	if (size <= 0) return;
	char* buffer = new char[size];
	rw::memcpy(buffer, src, size);
	rw::memcpy(dest, buffer, size);
	delete[] buffer;
}

void rw::memset(void* dest, const char& content, size_t size)
{
	for (size_t pos = 0; pos < size; pos++)
		((char*)dest)[pos] = content;
}
