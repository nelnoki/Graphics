#include "TriangleComponent.h"
#include "Game.h"

TriangleComponent::TriangleComponent(
	Game* game,
	std::vector<DirectX::XMFLOAT4> points, 
	std::vector<int> indeces,
	std::vector<UINT> strides, 
	std::vector<UINT> offsets, 
	LPCWSTR shader)
	: 
	GameComponent(game), 
	points(points), 
	indeces(indeces), 
	strides(strides), 
	offsets(offsets),
	shaderSource(shader),
	worldMatrix(DirectX::SimpleMath::Matrix::Identity) {}

void TriangleComponent::Initialize() {

	ID3DBlob* errorVertexCode = nullptr;

	auto res = D3DCompileFromFile(
		shaderSource,
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderByteCode,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->Display->hWnd, shaderSource, L"Missing Shader File", MB_OK);
		}
	}

	D3D_SHADER_MACRO Shader_Macros[] = {
								"TEST", "1",
								"TCOLOR", "float4(0.0f, 0.0f, 1.0f, 1.0f)",
								nullptr, nullptr };

	ID3DBlob* errorPixelCode;

	res = D3DCompileFromFile(
		shaderSource,
		Shader_Macros /*macros*/,
		nullptr /*include*/,
		"PSMain",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&pixelShaderByteCode,
		&errorPixelCode);

	game->Device->CreateVertexShader(
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		nullptr, &vertexShader);

	game->Device->CreatePixelShader(
		pixelShaderByteCode->GetBufferPointer(),
		pixelShaderByteCode->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game->Device->CreateInputLayout(
		inputElements,
		2,
		vertexShaderByteCode->GetBufferPointer(),
		vertexShaderByteCode->GetBufferSize(),
		&layout);


	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points.data();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);

	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->Device->CreateBuffer(&indexBufDesc, &indexData, &ib);


	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	res = game->Device->CreateRasterizerState(&rastDesc, &rastState);

	D3D11_BUFFER_DESC constBufDesc = {};
	constBufDesc.ByteWidth = sizeof(DirectX::SimpleMath::Matrix);
	constBufDesc.Usage = D3D11_USAGE_DEFAULT;
	constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer);

}

void TriangleComponent::Draw() {
	game->Context->RSSetState(rastState);

	game->Context->IASetInputLayout(layout);

	game->Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	game->Context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	game->Context->IASetVertexBuffers(0, 1, &vb, strides.data(), offsets.data());

	game->Context->VSSetShader(vertexShader, nullptr, 0);
	game->Context->PSSetShader(pixelShader, nullptr, 0);

	game->Context->UpdateSubresource(constantBuffer, 0, nullptr, &worldMatrix, 0, 0);
	game->Context->VSSetConstantBuffers(0, 1, &constantBuffer);

	game->Context->DrawIndexed(indeces.size(), 0, 0);
}

void TriangleComponent::DestroyResources() {
	layout->Release();

	vertexShader->Release();
	vertexShaderByteCode->Release();

	pixelShader->Release();
	pixelShaderByteCode->Release();

	rastState->Release();

	ib->Release();
	vb->Release();
	constantBuffer->Release();
}

void TriangleComponent::Update() {}

void TriangleComponent::Reload() {}