#pragma once
#ifndef BFDEVICERENDERERWIN32
#define BFDEVICERENDERERWIN32

#include "BFDeviceRenderer.h"
#include"BFContent.h"
#include<Windows.h>
#include<unordered_map>
#include<functional>

#include "imm.h"
#pragma comment(lib, "imm32.lib")

//static int screen_keys[512];
static int screen_exit = 0;

void screen_dispatch(void);

int BFContent::screen_keys[512] = { 0 };

static LRESULT screen_events(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: screen_exit = 1; break;
	case WM_KEYDOWN: 
		BFContent::screen_keys[wParam & 511] = 1; 
		break;
	//case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
	default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}



class IBFDeviceRendererWin32 : public IBFDeviceRenderer
{
public:

	IBFDeviceRendererWin32();

	~IBFDeviceRendererWin32();

	void Init(UINT bufferWidth, UINT bufferHeight, IBFCamera* camera) override;

	void Display() override;

	void DrawTest(int j);

private:

	///////////////////////////////////////////
	// Window and screen.
	
	HWND m_hWindowHandle;
	UINT mWindowWidth;
	UINT mWindowHeight;
	HDC m_hScreenDC;// The DC to draw into.
};

#endif // !IBFDEVICERENDERERWIN32