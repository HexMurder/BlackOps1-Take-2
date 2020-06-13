#include "DirectX.h"
#include <Psapi.h>
#include <string.h>
#include <string>


directx_t DirectX;
ID3DXFont* pFont;
LPD3DXFONT font_interface = NULL;
IDirect3D9Ex* p_Object = 0;
IDirect3DDevice9Ex* p_Device = 0;
D3DPRESENT_PARAMETERS p_Params;
RECT rc;
overlay_t Overlay;
target_t Target;


void DirectxFunctions::DirectXInit(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &DirectX.Object)))
	{
		MessageBox(NULL, "Failure to init Directx", "Title", MB_OK);
		Sleep(1000);
		exit(1);
	}
	ZeroMemory(&DirectX.Param, sizeof(DirectX.Param));
	DirectX.Param.Windowed = true;
	DirectX.Param.BackBufferFormat = D3DFMT_A8R8G8B8;
	DirectX.Param.BackBufferHeight = Overlay.Height;
	DirectX.Param.BackBufferWidth = Overlay.Width;
	DirectX.Param.EnableAutoDepthStencil = true;
	DirectX.Param.AutoDepthStencilFormat = D3DFMT_D16;
	DirectX.Param.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	DirectX.Param.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (FAILED(DirectX.Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX.Param, 0, &DirectX.Device)))
	{
		MessageBox(NULL, "Failure to Create Directx Device, Perhaps Wrong Game?", "Error", MB_OK);
		Sleep(100);
		exit(1);
	}

	D3DXCreateFont(DirectX.Device, 16, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &DirectX.Font);
	D3DXCreateFont(DirectX.Device, 12, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas", &DirectX.espFont);

	if (!DirectX.Line)
		D3DXCreateLine(DirectX.Device, &DirectX.Line);
}


void DirectxFunctions::RenderDirectX()
{
	DirectX.Device->BeginScene();
	if (GetForegroundWindow() == Target.Window)
	{
		//Draw Here
		Drawing::StringOutlined(1100, 3, "Prototype v1.0 by HexMurder",
			D3DCOLOR_ARGB(255, 32, 32, 32), D3DCOLOR_ARGB(255, 255, 128, 0), DirectX.Font);
	}
	DirectX.Device->EndScene();
	DirectX.Device->PresentEx(0, 0, 0, 0, 0);
	DirectX.Device->Clear(0, 0, D3DCLEAR_TARGET, 0, 1.0f, 0);
}

void MsgBoxAddy(uintptr_t addy)
{
	char szBuffer[1024];
	sprintf(szBuffer, "%02x", addy);
	MessageBox(NULL, szBuffer, "Title", MB_OK);

	//copy addy to clipboardd
	const char* output = szBuffer;
	const size_t len = strlen(output) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard(0);
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();
}

//void InitHooksAndValues()
//{
//	if (hackSettings.inGameStatus == 0)//dont do shit till we are in game
//	{
//		EntityHook.pattern = patternManager.entityHook;
//		EntityHook.mask = "xxxxxxxx";
//		EntityHook.hookSize = 8;
//		EntityHook.hookAddress = ProcFuncs::FindPattern(processName, EntityHook.pattern, EntityHook.mask);
//		EntityHook.hookJumpBack = EntityHook.hookAddress + EntityHook.hookSize;
//		if (EntityHook.hookAddress)
//			ProcFuncs::PlaceJMP((BYTE*)EntityHook.hookAddress, (uintptr_t)AsmEntityHook, EntityHook.hookSize);
//
//		RECT rect;
//		GetWindowRect(Overlay.Window, &rect);
//
//
//		//hackSettings.screenDimensions.x = rect.right - rect.left;
//		//hackSettings.screenDimensions.y = rect.top - rect.bottom;
//
//		offsetManager.noRecoil = ProcFuncs::FindPattern(processName, patternManager.noRecoil, "xxxxxxxxxxx");
//
//		RefreshValues();
//		hooksInitialized = true;
//	}
//}


void directx_t::Unload()
{
	//Not sure if we need to do this but,
	if (espFont)
	{
		espFont->Release();
		espFont = nullptr;
	}
	if (Font)
	{
		Font->Release();
		Font = nullptr;
	}
	if (FontWarning)
	{
		FontWarning->Release();
		FontWarning = nullptr;
	}
	if (Line)
	{
		Line->Release();
		Line = nullptr;
	}
	if (Object)
	{
		Object->Release();
		Object = nullptr;
	}
	if (Device)
	{
		Device->Release();
		Device = nullptr;
	}

	if (pFont)
	{
		pFont->Release();
		pFont = nullptr;
	}
	if (font_interface)
	{
		font_interface->Release();
		font_interface = nullptr;
	}
	if (p_Object)
	{
		p_Object->Release();
		p_Object = nullptr;
	}
	if (p_Device)
	{
		p_Device->Release();
		p_Device = nullptr;
	}
	ZeroMemory(&DirectX.Param, sizeof(DirectX.Param));
}
