#pragma once
#include "Containers.h"
#include "rwapi.h"

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

	static String FromInteger(int inInteger, int MinDigits = 1);
	
	String operator+(const String& Other) const;
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