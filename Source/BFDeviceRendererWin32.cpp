#include "BFDeviceRendererWin32.h"

IBFDeviceRendererWin32::IBFDeviceRendererWin32()
{
}


IBFDeviceRendererWin32::~IBFDeviceRendererWin32()
{
}

void IBFDeviceRendererWin32::Init(UINT bufferWidth, UINT bufferHeight, IBFCamera* camera)
{
	////////////////////////////////
	// Initialize window.

	// Configure window class.
	WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
		NULL, NULL, NULL, NULL, "BFWindow" };
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hInstance = GetModuleHandle(NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);// set arrow.

	if (!RegisterClass(&wc)) MSG("Window register FAILED.");// Register window class.

	// Create handle of window.
	HWND hWindowHandle = CreateWindow("BFWindow", "Black Flag on.",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
	if (hWindowHandle == NULL) MSG("Window handle create FAILED.");
	m_hWindowHandle = hWindowHandle;
	mWindowWidth = bufferWidth;
	mWindowHeight = bufferHeight;

	// Get window DC.
	HDC hDC = GetDC(hWindowHandle);
	m_hScreenDC = CreateCompatibleDC(hDC);// Create buffer buffer.
	ReleaseDC(hWindowHandle, hDC);// realese useless DC.
	
	// Configure bitmap info.
	BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER), bufferWidth, bufferHeight, 1, 24, BI_RGB,
		bufferWidth * bufferHeight * 3, 0, 0, 0, 0 } };// 24 bits color, 3 channels RGB for 3 byte.
	// Color bits : /R:8/G:8/B:8.

	// Create bitmap.
	HBITMAP hBmpNew = NULL;
	HBITMAP hBmpOld = NULL;
	LPVOID values;// bitmap's pixel values.

	// Allocate spaces for bmp pixels.
	hBmpNew = CreateDIBSection(m_hScreenDC, &bi, DIB_RGB_COLORS, &values, 0, 0);
	if (hBmpNew == NULL) return;
	m_pFrameBuffer = (unsigned char*)values;

	// Select created bitmap.
	hBmpOld = (HBITMAP)SelectObject(m_hScreenDC, hBmpNew);// Select new bmp into screenDC replace old bmp.

	// Adjust window shape, position.
	RECT rect = { 0, 0, bufferWidth, bufferHeight };
	AdjustWindowRect(&rect, GetWindowLong(hWindowHandle, GWL_STYLE), 0);
	int wx, wy, sx, sy;
	wx = rect.right - rect.left;
	wy = rect.bottom - rect.top;
	sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
	sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
	if (sy < 0) sy = 0;// Too large.
	SetWindowPos(hWindowHandle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
	SetForegroundWindow(hWindowHandle);

	memset(m_pFrameBuffer, 0, bufferWidth*bufferHeight * 3);// Set memory with 0.

	ShowWindow(hWindowHandle, SW_NORMAL);

	ImmDisableIME(0);// Disable IME.
	//memset(screen_keys, 0, sizeof(int) * 512);
	screen_dispatch();// Deal with message before rendering.

	IBFDeviceRenderer::Init(bufferWidth, bufferHeight, camera);
}

void IBFDeviceRendererWin32::Display()
{
	// From color buffer to frame buffer.
	IBFDeviceRenderer::mFunction_UpdateFrameBufer();

	// Display frame buffer.
	HDC hCurrentDC = GetDC(m_hWindowHandle);
	BitBlt(hCurrentDC, 0, 0, 800, 600, m_hScreenDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWindowHandle, hCurrentDC);

	screen_dispatch();// Waitting next message.
}

void IBFDeviceRendererWin32::DrawTest(int J)
{
	for (size_t j = 0; j < UINT(J); j++)
	{
		for (size_t i = 0; i < 400 * 3; i += 3)
		{
			m_pFrameBuffer[j*800*3 +i] = 0xFF;
			m_pFrameBuffer[j * 800 * 3 +i + 1] = 0xF5;
			m_pFrameBuffer[j * 800 * 3 + i + 2] = 0x00;
		}
	}
	
}

void screen_dispatch(void) {
	MSG msg;
	while (1) {
		if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
		if (!GetMessage(&msg, NULL, 0, 0)) break;
		DispatchMessage(&msg);
	}
}
