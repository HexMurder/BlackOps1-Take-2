#pragma once
#include "ProcFuncs.h"
#include <TlHelp32.h>

void ProcFuncs::CheckDWM()
{
	BOOL Enabled;
	HRESULT Result = DwmIsCompositionEnabled(&Enabled);
	if (!SUCCEEDED(Enabled) && Enabled)
	{
		printf("Please make sure your Windows Aero is enabled!\n\nSupported Systems:\n* Windows Vista\n* Windows 7\n* Windows 8 / 8.1", "Warning!");
		Sleep(1000);
	}
}

uintptr_t ProcFuncs::GetModuleBase(uintptr_t dwProcessIdentifier, TCHAR *lpszModuleName)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessIdentifier);
	uintptr_t dwModuleBaseAddress = 0;
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 ModuleEntry32 = { 0 };
		ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &ModuleEntry32))
		{
			do
			{
				if (strcmp(ModuleEntry32.szModule, lpszModuleName) == 0)
				{
					dwModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &ModuleEntry32));
		}
		CloseHandle(hSnapshot);
	}
	return dwModuleBaseAddress;
}

HANDLE ProcFuncs::GetHandle()
{
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	return pHandle;
}

void ProcFuncs::memNop(BYTE* dst, unsigned int size)
{
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);
	memset(dst, 0x90, size);
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void ProcFuncs::memRestore(BYTE* dst, char* originalPattern, unsigned int size)
{
	DWORD dwOldProtect, dwBkup;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	for (int i = 0; i < size; i++)
	{
		*(dst + i) = originalPattern[i];
	}
	VirtualProtect(dst, size, dwOldProtect, &dwBkup);
}


void ProcFuncs::PlaceJMP(BYTE* Address, uintptr_t jumpTo, uintptr_t length)
{
	DWORD dwOldProtect, dwBkup, dwRelAddr;

	//give that address read and write permissions and store the old permissions at oldProtection
	VirtualProtect(Address, length, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// Calculate the "distance" we're gonna have to jump - the size of the JMP instruction
	dwRelAddr = (uintptr_t)(jumpTo - (uintptr_t)Address) - 5;

	// Write the JMP opcode @ our jump position...
	*Address = 0xE9;

	// Write the offset to where we're gonna jump
	//The instruction will then become JMP ff002123 for example
	*((uintptr_t*)(Address + 0x1)) = dwRelAddr;

	// Overwrite the rest of the bytes with NOPs
	//ensuring no instruction is Half overwritten(To prevent any crashes)
	for (uintptr_t x = 0x5; x < length; x++)
		*(Address + x) = 0x90;

	// Restore the default permissions
	VirtualProtect(Address, length, dwOldProtect, &dwBkup);
}


MODULEINFO ProcFuncs::GetModuleInfo(char* szModule)
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandle(szModule);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

uintptr_t ProcFuncs::FindPattern(char* module, char* pattern, char* mask)
{
	//Get all module related information
	MODULEINFO mInfo = GetModuleInfo(module);

	//Assign our base and module size
	//Having the values right is ESSENTIAL, this makes sure
	//that we don't scan unwanted memory and leading our game to crash
	uintptr_t base = (uintptr_t)mInfo.lpBaseOfDll;
	uintptr_t size = (uintptr_t)mInfo.SizeOfImage;

	//Get length for our mask, this will allow us to loop through our array
	uintptr_t patternLength = (uintptr_t)strlen(mask);

	for (uintptr_t i = 0; i < size - patternLength; i++)
	{
		bool found = true;
		for (uintptr_t j = 0; j < patternLength; j++)
		{
			//if we have a ? in our mask then we have true by default, 
			//or if the bytes match then we keep searching until finding it or not
			found &= mask[j] == '?' || pattern[j] == *(char*)(base + i + j);
		}

		//found = true, our entire pattern was found
		//return the memory addy so we can write to it
		if (found)
		{
			return base + i;
		}
	}

	return NULL;
}

template <class TypeValue>
TypeValue ProcFuncs::ReadMemory(uintptr_t Address)
{
	TypeValue Value;
	ReadProcessMemory(pHandle, (LPCVOID*)Address, &Value, sizeof(TypeValue), 0);
	return Value;
}

template <class TypeValue>
TypeValue ProcFuncs::WriteMemory(uintptr_t Address)
{
	TypeValue Value;
	WriteProcessMemory(pHandle, (LPCVOID*)Address, &Value, sizeof(TypeValue), 0);
	return Value;
}

