#pragma once
#include "TriangleComponent.h"

class SphereComponent : public TriangleComponent {
public:
	SphereComponent(
		Game* game,
		float radius,
		int segments,
		std::vector<UINT> strides = { 32 },
		std::vector<UINT> offsets = { 0 },
		LPCWSTR shader = L"./Shaders/MyVeryFirstShader.hlsl");

private:
	void GenerateSpherePoints(std::vector<DirectX::XMFLOAT4>& points, std::vector<int>& indeces, float radius,
		int segments);
};

