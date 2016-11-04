#ifdef DLLLIB
#define RWGUI_API __declspec(dllexport)
#else
#define RWGUI_API __declspec(dllimport)
// Overriding windows api defines
#undef CreateWindow
#endif

#pragma warning ( disable : 4267 4244 4390 4101 )

#ifdef _DEBUG
	//#define ENABLE_CRT_MEMORYLEAK_DEBUG
#endif

#ifdef ENABLE_CRT_MEMORYLEAK_DEBUG
	#ifdef _DEBUG
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
	#endif

	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif

#include <rwresources.h>