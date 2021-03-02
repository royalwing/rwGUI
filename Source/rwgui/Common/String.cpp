#include "String.h"
#include <Common/Memory.h>
#include <cstdlib>

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
		rw::memcpy(Data, charArray, sizeof(char)*len);
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
	rw::memset(Data, ' ', Length);
	Data[Length] = '\0';
}

size_t String::Length() const
{
	if (Data == nullptr) return 0;
	size_t count = 0;
	for(size_t pos = 0; Data[pos]!='\0'; pos++)
	{
		count = pos+1;
	}
	return count;
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

String String::FromInteger(int inInteger, int MinDigits)
{
	

	char buffer[256];
	_itoa(inInteger, buffer, 10);
	String result(buffer);
	while(result.Length()<MinDigits)
	{
		result = String("0") + result;
	}
	return result;
}


String String::operator+(const String& Other) const
{
	if (Other.Length() == 0) return *this;
	if (this->Length() == 0) return Other;
	String NewString;
	if (NewString[NewString.Size() - 1] == '\0')
		NewString.RemoveAt(NewString.Size() - 1);
	NewString.Append(*this);
	if (NewString[NewString.Size() - 1] == '\0')
		NewString.RemoveAt(NewString.Size() - 1);
	NewString.Append(Other);
	return NewString;
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
		rw::memcpy(Data, charArray, sizeof(wchar_t) * len);
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
	rw::memset(Data, ' ', Length);
	Data[Length] = L'\0';
}

size_t WideString::Length() const
{
	if (Data == nullptr) return 0;
	size_t count = 0;
	for (size_t pos = 0; Data[pos] != '\0'; pos++)
	{
		count = pos+1;
	}
	return count;
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
