#ifndef FUNCTIONS
#define FUNCTIONS

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include "Overlay.h"
#include <time.h>
#include "modulemgr.h"


struct Vector4
{
	float x, y, z, w;
};
struct Vector3
{
	float x, y, z;
};
struct Vector2
{
	int x, y;
};

namespace ProcFuncs
{
	template < class TypeValue > TypeValue ReadMemory(DWORD Address);
	template < class TypeValue > TypeValue WriteMemory(DWORD Address);
	void CheckDWM();
	DWORD GetModuleBase(DWORD dwProcessIdentifier, TCHAR *lpszModuleName);
	HANDLE GetHandle();
	void memNop(BYTE* dst, unsigned int size);
	void memRestore(BYTE* dst, char* originalPattern, unsigned int size);
	MODULEINFO GetModuleInfo(char* szModule);
	DWORD64 FindPattern(char* module, char* pattern, char* mask);
	void PlaceJMP(BYTE* Address, DWORD jumpTo, DWORD length = 5);
}


#endif



