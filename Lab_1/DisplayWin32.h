#pragma once

#include <windows.h>
#include <WinUser.h>
#include <iostream>


class Game;
class InputDevice;


class DisplayWin32 {

public:
	DisplayWin32(Game* g, LPCWSTR applicationName, int screenWidth, int screenHeight, HINSTANCE hInst = GetModuleHandle(nullptr));

	int ClientHeight;
	int ClientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;
	Game* game;
};