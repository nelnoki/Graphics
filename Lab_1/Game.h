#pragma once
#include <windows.h>
#include <WinUser.h>
#include <wrl.h>
#include <iostream>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
class Game
{
public:
	backBuffer;
	ID3D11DeviceContext* Context;
	DebugAnnotation;
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Instance;
	Name;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	RenderSRV;
	RenderView;
	ScreenResized;
	StartTime;
	IDXGISwapChain* SwapChain;
	float TotalTime;

	CreateBackBuffer;
	DestroyResources;
	Draw;
	EndFrame;
	Exit;
	Game;
	Init;
	MessageHandler;
	PrepareFrame;
	PrepareResources;
	RestoreTargets;
	Run;
	Udpate;
	UpdateInternal
};

