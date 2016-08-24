#ifdef DLLLIB
#define RWGUI_API __declspec(dllexport)
#else
#define RWGUI_API __declspec(dllimport)
// Overriding windows api defines
#undef CreateWindow
#endif