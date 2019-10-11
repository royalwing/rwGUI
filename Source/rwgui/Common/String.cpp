#define DLLEXPORT
#include "String.h"
#include <cstdlib>

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
	for(size_t pos = 0; pos < size; pos++)
		((char*)(dest))[pos] = ((char*)(src))[pos];
}

void rw::memmove(void* dest,const void* src, size_t size)
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

using namespace rw;

String::String()
	: String(size_t(0))
{
}

String::String(const char* charArray)
{
	const size_t len = rw::strlen(charArray);
	if(len>0)
	{
		Init(len + 1);
		memcpy(Data, charArray, sizeof(char)*len);
		Data[len] = '\0';
	};
}

String::String(const String& Other)
	: String(Other.Data)
{
	
}

String::String(const size_t Length)
{
	Init(Length+size_t(1));
	memset(Data, ' ', Length);
	Data[Length] = '\0';
}

size_t String::Length() const
{
	return strlen(Data);
}

bool String::IsEmpty() const
{
	return Length() == 0;
}

bool String::operator==(const String& Other) const
{
	if (Other.Length() != Length()) return false;
	for (size_t pos = 0; pos < Length(); pos++)
		if (Other.Data[pos] != Data[pos])
			return false;
	return true;
}

WideString String::ToWideString() const
{
	WideString result;
	result.Init(Length()+1);
	if(Length()>0) mbstowcs(result, Data, Length());
	return result;
}

WideString::WideString()
	: WideString(size_t(0))
{
}

WideString::WideString(const wchar_t* charArray)
{
	const size_t len = rw::strlen(charArray);
	if (len > 0)
	{
		Init(len + 1);
		memcpy(Data, charArray, sizeof(wchar_t) * len);
		Data[len] = L'\0';
	};
}

WideString::WideString(const WideString& Other)
	: WideString(Other.Data)
{
}

WideString::WideString(const size_t Length)
{
	Init(Length + 1);
	memset(Data, ' ', Length);
	Data[Length] = L'\0';
}

size_t WideString::Length() const
{
	return strlen(Data);
}

bool WideString::IsEmpty() const
{
	return Length() > 0;
}

bool WideString::operator==(const WideString& Other) const
{
	if (Other.Length() != Length()) return false;
	for(size_t pos = 0; pos < Length();pos++)
		if (Other.Data[pos] != Data[pos])
			return false;
	return true;
}

String WideString::ToString() const
{
	String result;
	result.Init(Length());
	if (Length() > 0) mbstowcs(Data, result, Length());
	return result;
};
