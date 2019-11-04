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

void screen_dispatch(void);

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