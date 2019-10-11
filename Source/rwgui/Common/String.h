#pragma once
#include "Containers.h"
#include "rwapi.h"

namespace rw
{
	size_t strlen(const char* str);
	size_t strlen(const wchar_t* str);
	void memcpy(void* dest,const void* src, size_t size);
	void memmove(void* dest,const void* src, size_t size);
	void memset(void* dest, const char& content, size_t size);
}

class RWGUI_API String : public List<char>
{
public:
	String();
	String(const char* charArray);
	String(const String& Other);
	String(const size_t Length);

	explicit operator const char * const () const { return Data; };

	size_t Length() const;
	bool IsEmpty() const;

	bool operator==(const String& Other) const;

	class WideString ToWideString() const;
};

class RWGUI_API WideString : public List<wchar_t>
{
public:
	WideString();
	WideString(const wchar_t* charArray);
	WideString(const WideString& Other);
	WideString(const size_t Length);

	size_t Length() const;
	bool IsEmpty() const;

	bool operator==(const WideString& Other) const;

	class String ToString() const;
};