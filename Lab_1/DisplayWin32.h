#pragma once

#include <windows.h>
#include <WinUser.h>
#include <iostream>


class Game;
class InputDevice;


class DisplayWin32 {

public:
	DisplayWin32(LPCWSTR applicationName, int screenWidth, int screenHeight, HINSTANCE hInst = GetModuleHandle(nullptr));

	int ClientHeight;
	int ClientWidth;
	HINSTANCE hInstance;
	HWND hWnd;
	WNDCLASSEX wc;

	//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};