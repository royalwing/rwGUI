#pragma once
#include <rwapi.h>

namespace rw
{
	RWGUI_API size_t strlen(const char* str);
	RWGUI_API size_t strlen(const wchar_t* str);
	RWGUI_API void memcpy(void* dest, const void* src, size_t size);
	RWGUI_API void memmove(void* dest, const void* src, size_t size);
	RWGUI_API void memset(void* dest, const char& content, size_t size);
}
