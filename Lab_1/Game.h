#pragma once

#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <chrono>

#include <vector>
#include "GameComponent.h"
#include "DisplayWin32.h"
#include "InputDevice.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")


class Game {

private:
	void createBackBuffer();

protected:
	virtual void DestroyResources();
	virtual void Draw();
	virtual void EndFrame();
	virtual void Initialize();
	virtual void PrepareFrame();
	virtual void PrepareResources();
	virtual void Update();
	virtual void UpdateInternal();

public:
	Game(LPCWSTR name, int screenWidth, int screenHeight);

	ID3D11Texture2D* BackBuffer;
	ID3D11DeviceContext* Context;
	int DebugAnnotation;
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	HINSTANCE Instance;
	LPCWSTR Name;
	ID3D11Texture2D* RenderSRV;
	ID3D11RenderTargetView* RenderView;
	IDXGISwapChain* SwapChain;
	int ScreenResized;
	std::chrono::time_point<std::chrono::steady_clock> PrevTime;
	float StartTime;
	float TotalTime;
	float DeltaTime;
	unsigned int FrameCount;
	std::vector<GameComponent*> Components;
	DisplayWin32* Display;
	InputDevice* InDevice;
	
	void Exit();
	void MessageHandler(MSG& msg, bool& isExitRequested);
	void RestoreTargets();
	void Run();
	
};

