#include "main.h"

char* processName = "BlackOps.exe";

void Unload()
{
	DirectX.Unload();
	OverlayFunctions::Unload();
}

LRESULT CALLBACK WinProcedure(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch (Message)
	{
	case WM_PAINT:
		DirectxFunctions::RenderDirectX();
		break;

	case WM_DESTROY:
		PostQuitMessage(1);
		break;

	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	//Credits to Icew0lf from MPGH for the Base, Directx overlay, and for helping me with numerous issues.
	ProcFuncs::CheckDWM();
	OverlayFunctions::GetTargetWindow();
	OverlayFunctions::CreateClass(WinProcedure, "overlay");
	OverlayFunctions::CreateWindowOverlay();
	DirectxFunctions::DirectXInit(Overlay.Window);

	while (!GetAsyncKeyState(VK_DELETE))
	{
		if (PeekMessage(&Overlay.Message, Overlay.Window, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&Overlay.Message);
			TranslateMessage(&Overlay.Message);
		}
		Sleep(1);
		OverlayFunctions::GetTargetWindow();
	}
  	
	Unload();
	Sleep(1000);
	 
	FreeLibraryAndExitThread(hInstance, 1);
	return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hInstance = hModule;
		CreateThread(nullptr, NULL, MainThread, hModule, NULL, nullptr);
	}
	return TRUE;
}


