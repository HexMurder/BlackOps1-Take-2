#pragma once
#include <Windows.h>

class Hooks
{
public:
	char* pattern;
	char* mask;
	int hookSize = 0;

	uintptr_t hookObjStart = 0;
	uintptr_t hookJumpBack = 0;
	uintptr_t hookAddress = 0;
};