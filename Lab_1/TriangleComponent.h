#pragma once
#include <wrl.h>
#include <iostream>
#include <algorithm>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <vector>
#include <SimpleMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX::SimpleMath;

#include "GameComponent.h"


class TriangleComponent : public GameComponent {
public:
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;
	
	TriangleComponent(
		Game* game, 
		std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }, 
		std::vector<int> indeces = { 0,1,2, 0,1,3 },
		std::vector<UINT> strides = { 32 }, 
		std::vector<UINT> offsets = { 0 },
		LPCWSTR shader = L"./Shaders/MyVeryFirstShader.hlsl" );

	DirectX::SimpleMath::Matrix worldMatrix;
	DirectX::SimpleMath::Matrix viewMatrix;
	DirectX::SimpleMath::Matrix projMatrix;

	std::vector<DirectX::XMFLOAT4> points;

private:

	LPCWSTR shaderSource;
	ID3D11InputLayout* layout;
	ID3D11PixelShader* pixelShader;
	ID3DBlob* pixelShaderByteCode;
	ID3D11RasterizerState* rastState;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vertexShaderByteCode;
	std::vector<int> indeces;
	std::vector<UINT> strides;
	std::vector<UINT> offsets;
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;

	ID3D11Buffer* constantBuffer;
};