#pragma once
#include "TriangleComponent.h"

class CubeComponent : public TriangleComponent {
public:
	CubeComponent(
		Game* game,
		float size,
		std::vector<DirectX::XMFLOAT4> points = {

		DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f,  1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),

		DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f,  1.0f, -1.0f, 1.0), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)

		},

		std::vector<int> indeces = {
			// Front
			0, 1, 2,  0, 2, 3,
			// BackS
			4, 6, 5,  4, 7, 6,
			// Top
			3, 2, 6,  3, 6, 7,
			// Bottom
			0, 5, 1,  0, 4, 5,
			// Left
			0, 3, 7,  0, 7, 4,
			// Right
			1, 5, 6,  1, 6, 2
		},

		std::vector<UINT> strides = { 32 },
		std::vector<UINT> offsets = { 0 },
		LPCWSTR shader = L"./Shaders/MyVeryFirstShader.hlsl")
		: TriangleComponent(game, points, indeces, strides, offsets, shader), size(size) {

		for (int i = 0; i < points.size() / 2; i + 2) {
			points[i].x *= size;
			points[i].y *= size;
			points[i].z *= size;
		}
	};

	float size;
};

