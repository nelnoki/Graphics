#pragma once
#include "TriangleComponent.h"

class CubeComponent : public TriangleComponent {
public:
	CubeComponent(
		Game* game,
		float size,
		std::vector<DirectX::XMFLOAT4> points = {

		DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, .0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(.0f, .0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f,  1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(.0f, .0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(.0f, .0f, 1.0f, 1.0f),

		DirectX::XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), DirectX::XMFLOAT4(.0f, 1.0f, .0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0), DirectX::XMFLOAT4(.0f, 1.0f, .0f, 1.0f),
		DirectX::XMFLOAT4(1.0f,  1.0f, -1.0f, 1.0), DirectX::XMFLOAT4(.0f, 1.0f, .0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f,  1.0f, -1.0f, 1.0f),  DirectX::XMFLOAT4(.0f, 1.0f, .0f, 1.0f)

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
		: TriangleComponent(game, points, indeces, strides, offsets, shader) 
		{
			float half = size / 2.0f;

			std::vector<DirectX::XMFLOAT4> scaledPoints = {
				
				{ -half, -half,  half, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
				{  half, -half,  half, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
				{  half,  half,  half, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
				{ -half,  half,  half, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },

				{ -half, -half, -half, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
				{  half, -half, -half, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
				{  half,  half, -half, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
				{ -half,  half, -half, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f },
			};

			this->points = scaledPoints;
		};
};

