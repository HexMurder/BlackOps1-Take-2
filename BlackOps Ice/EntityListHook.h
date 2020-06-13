#pragma once
#include "Entity.h"
#include "Hooks.h"


int id;
bool alreadyThere = false;
int maxEnts = 100;

Hooks EntityHook;


__declspec(naked) void AsmEntityHook()
{
	__asm
	{
		mov eax, [ecx]
		mov EntityHook.hookObjStart, eax
		fld dword ptr[eax + 0x11c]
		pushad
	}

	__asm
	{
		mov eax, EntityHook.hookObjStart
		mov[entsptr], eax
	}

	if (entsptr == nullptr)
		goto GIVE_UP;
	

	alreadyThere = false;

	for (id = 0; id < maxEnts; id++)
	{
		if (ents[id] == entsptr)
		{
			alreadyThere = true;
			break;
		}
	}


	if (alreadyThere)
	{
		goto GIVE_UP;
	}
	else
	{
		for (id = 0; id < maxEnts; id++)
		{
			if (ents[id] == 0)
			{
				ents[id] = entsptr;
				break;
			}
		}
	}

GIVE_UP:
	__asm {
		popad
		jmp[EntityHook.hookJumpBack]
	}
}