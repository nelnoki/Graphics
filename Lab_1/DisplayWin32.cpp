#include "DisplayWin32.h"
#include "Game.h"

DisplayWin32::DisplayWin32 (Game* g, LPCWSTR applicationName, int screenWidth, int screenHeight, HINSTANCE hInst)
	: game(g), ClientWidth(screenWidth), ClientHeight(screenHeight), hInstance(hInst) {

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = Game::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	RECT windowRect = { 0, 0, static_cast<LONG>(ClientWidth), static_cast<LONG>(ClientHeight) };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

	auto dwStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX | WS_THICKFRAME;

	auto posX = (GetSystemMetrics(SM_CXSCREEN) - ClientWidth) / 2;
	auto posY = (GetSystemMetrics(SM_CYSCREEN) - ClientHeight) / 2;

	hWnd = CreateWindowEx(
		WS_EX_APPWINDOW, 
		applicationName, 
		applicationName,
		dwStyle,
		posX, posY,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr, nullptr, hInstance, game);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	ShowCursor(true);

	RAWINPUTDEVICE Rid[1];

	Rid[0].usUsagePage = 0x01;          // HID_USAGE_PAGE_GENERIC
	Rid[0].usUsage = 0x02;              // HID_USAGE_GENERIC_MOUSE
	Rid[0].dwFlags = 0;
	Rid[0].hwndTarget = 0;

	if (RegisterRawInputDevices(Rid, 1, sizeof(Rid[0])) == FALSE)
	{
		// Registration failed. Call GetLastError for the cause of the error
	}
}