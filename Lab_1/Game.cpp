#include "Game.h"
#include "Tools/DataProcesser.h"

Game::Game(LPCWSTR name, int screenWidth, int screenHeight) {
	Instance = GetModuleHandle(nullptr);
	Display = new DisplayWin32(this, name, screenWidth, screenHeight, Instance);
	InDevice = new InputDevice(this);

	Camera = new ::Camera();
	Camera->AspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

}

void Game::PrepareResources() {
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
		D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT,
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

	DepthStencilBuffer = nullptr;
	DepthStencilView = nullptr;

	CreateDepthBuffer();

	DataProcesser::Initialize(this);

	D3D11_SAMPLER_DESC samplerStateDesc = {};
	samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerStateDesc.MinLOD = 0.0f;
	samplerStateDesc.MaxLOD = D3D11_FLOAT32_MAX;

	res = Device->CreateSamplerState(&samplerStateDesc, &SamplerState);


	CD3D11_RASTERIZER_DESC rastDesc = {};

	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = true;
	rastDesc.DepthClipEnable = true;

	res = Device->CreateRasterizerState(&rastDesc, &RastState);

	Context->RSSetState(RastState);

}

void Game::createBackBuffer() {
	auto res = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);	// __uuidof(ID3D11Texture2D)
	res = Device->CreateRenderTargetView(BackBuffer, nullptr, &RenderView);
}

void Game::CreateDepthBuffer()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = Display->ClientWidth;
	depthStencilDesc.Height = Display->ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;


	Device->CreateTexture2D(&depthStencilDesc, nullptr, &DepthStencilBuffer);

	Device->CreateDepthStencilView(DepthStencilBuffer, nullptr, &DepthStencilView);
}

void Game::Initialize() {
	for (GameComponent* component : Components) {
		component->Initialize();
	}
}

void Game::Run() {
	PrepareResources();
	Initialize();
	InitTimer();
	
	TotalTime = 0;
	FrameCount = 0;
	
	MSG msg = {};
	bool isExitRequested = false;

	while (!isExitRequested) {
		MessageHandler(msg, isExitRequested);
		UpdateTimer();
		Update();
	}
	Exit();
}

void Game::Draw() {
	//Context->OMSetRenderTargets(1, &RenderView, nullptr);

	for (GameComponent* component : Components) {
		component->Draw();
	}

	//Context->OMSetRenderTargets(0, nullptr, nullptr);

	//SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
}
void Game::InitTimer() {
	PrevTime = std::chrono::steady_clock::now();
}

void Game::UpdateTimer() {
	auto	curTime = std::chrono::steady_clock::now();
	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - PrevTime).count() / 1000000.0f;
	PrevTime = curTime;

	TotalTime += DeltaTime;
	TotalestTime += DeltaTime;
	FrameCount++;

	if (TotalTime > 1.0f) {
		float fps = FrameCount / TotalTime;

		TotalTime -= 1.0f;

		WCHAR text[256];
		swprintf_s(text, TEXT("FPS: %f"), fps);
		SetWindowText(Display->hWnd, text);

		FrameCount = 0;
	}
}

void Game::Update() {
	UpdateInternal();
	PrepareFrame();
	Draw();
	EndFrame();
}

void Game::UpdateInternal() {
	for (auto component : Components) {
		component->Update();
	}
}

void Game::PrepareFrame() {
		/*Context->ClearState();
		if (DepthStencilView != nullptr)
			Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


		D3D11_VIEWPORT viewport = {};
		viewport.Width = static_cast<float>(Display->ClientWidth);
		viewport.Height = static_cast<float>(Display->ClientHeight);
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.MinDepth = 0;
		viewport.MaxDepth = 1.0f;

		Context->RSSetViewports(1, &viewport);*/


		Context->ClearState();

		Context->RSSetState(RastState);

		Context->OMSetRenderTargets(1, &RenderView, DepthStencilView);

		Context->VSSetShader(DataProcesser::GetVertexShader("spinny"), nullptr, 0);
		Context->PSSetShader(DataProcesser::GetPixelShader("spinny"), nullptr, 0);

		Context->PSSetSamplers(0, 1, &SamplerState);

		float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		Context->ClearRenderTargetView(RenderView, color);

		Context->ClearDepthStencilView(DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void Game::EndFrame() {
	//InDevice->EndFrame();

	Context->OMSetRenderTargets(0, nullptr, nullptr);

	SwapChain->Present(1, /*DXGI_PRESENT_DO_NOT_WAIT*/ 0);
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

	DepthStencilView->Release();
	DepthStencilBuffer->Release();
}

void Game::Exit() {
	DestroyResources();
}

Game::~Game() {
	DestroyResources();
}

void Game::RestoreTargets() {}

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

LRESULT CALLBACK Game::WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	Game* game;
	if (umessage == WM_NCCREATE) {
		game = static_cast<Game*>(reinterpret_cast<CREATESTRUCT*>(lparam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game))) {
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else {
		game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	switch (umessage) {
		case WM_KEYDOWN: {
			// If a key is pressed send it to the input object so it can record that state.
			//std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

			if (static_cast<unsigned int>(wparam) == 27) PostQuitMessage(0);
			return 0;
		}

		case WM_INPUT: {
			UINT dwSize = 0;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lparam), RID_INPUT, nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			if (lpb == nullptr) {
				return 0;
			}

			if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(lpb);

			if (raw->header.dwType == RIM_TYPEKEYBOARD) {
				//printf(" Kbd: make=%04i Flags:%04i Reserved:%04i ExtraInformation:%08i, msg=%04i VK=%i \n",
				//	raw->data.keyboard.MakeCode,
				//	raw->data.keyboard.Flags,
				//	raw->data.keyboard.Reserved,
				//	raw->data.keyboard.ExtraInformation,
				//	raw->data.keyboard.Message,
				//	raw->data.keyboard.VKey);

				game->InDevice->OnKeyDown({
					raw->data.keyboard.MakeCode,
					raw->data.keyboard.Flags,
					raw->data.keyboard.VKey,
					raw->data.keyboard.Message
					});
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE) {
				//printf(" Mouse: X=%04d Y:%04d \n", raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				game->InDevice->OnMouseMove({
					raw->data.mouse.usFlags,
					raw->data.mouse.usButtonFlags,
					static_cast<int>(raw->data.mouse.ulExtraInformation),
					static_cast<int>(raw->data.mouse.ulRawButtons),
					static_cast<short>(raw->data.mouse.usButtonData),
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY
					});
			}

			delete[] lpb;
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}

		default: {
			return DefWindowProc(hwnd, umessage, wparam, lparam);
		}
	}
}
