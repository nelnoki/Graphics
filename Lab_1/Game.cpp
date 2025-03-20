#include "Game.h"

Game* Game::GameInstance = nullptr;

void Game::createBackBuffer() {
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);	// __uuidof(ID3D11Texture2D)
	Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderView);
}

void Game::Initialize(LPCWSTR name, int screenWidth, int screenHeight) {
	Name = name;
	Display = new DisplayWin32(name, screenWidth, screenHeight);
	InDevice = new InputDevice(GetInstance());

	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->hWnd;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	auto res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);

	if (FAILED(res))
	{
		// Well, that was unexpected
	}

	createBackBuffer();
}

void Game::PrepareResources() {

}

void Game::Draw() {
	Context->OMSetRenderTargets(1, &RenderView, nullptr);

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	Context->ClearRenderTargetView(RenderView, color);

	for (GameComponent* component : Components) {
		component->Draw();
	}

	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}

void Game::PrepareFrame() {
		Context->ClearState();

		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(Display->ClientWidth);
		viewport.Height = static_cast<float>(Display->ClientHeight);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		Context->RSSetViewports(1, &viewport);
}
void Game::EndFrame() {}

void Game::Update() {
	PrepareFrame();

	auto	curTime = std::chrono::steady_clock::now();
	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	TotalTime += DeltaTime;
	FrameCount++;

	if (TotalTime > 1.0f) {
		float fps = FrameCount / TotalTime;

		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->hWnd, text);

		FrameCount = 0;
	}

	UpdateInternal();
	Draw();
	EndFrame();
}

void Game::UpdateInternal() {}

void Game::Run() {

	for (GameComponent* component : Components) {
		component->Initialize();
	}

	PrevTime = std::chrono::steady_clock::now();
	TotalTime = 0;
	FrameCount = 0;
	
	MSG msg = {};
	bool isExitRequested = false;

	while (!isExitRequested) {
		MessageHandler(msg, isExitRequested);
		Update();
	}
	Exit();
}

void Game::DestroyResources() {
	delete Display;
	delete InDevice;

	for (GameComponent* component : Components) {
		component->DestroyResources();
		delete component;
	}

	Context->Release();
	Device->Release();

	SwapChain->Release();

	BackBuffer->Release();
	RenderView->Release();
}

void Game::Exit() {
	DestroyResources();
}


void Game::MessageHandler(MSG &msg, bool &isExitRequested)
{
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If windows signals to end the application then exit out.
	if (msg.message == WM_QUIT) {
		isExitRequested = true;
	}
}

void Game::RestoreTargets() {}