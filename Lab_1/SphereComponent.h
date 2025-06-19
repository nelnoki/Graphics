//#pragma once
//#include "TriangleComponent.h"
//
//class SphereComponent : public TriangleComponent {
//public:
//	SphereComponent(
//		Game* game,
//		float radius,
//		int segments,
//		bool color,
//		std::vector<UINT> strides = { 32 },
//		std::vector<UINT> offsets = { 0 },
//		LPCWSTR shader = L"./Shaders/MyVeryFirstShader.hlsl");
//
//private:
//	void GenerateSpherePoints(std::vector<DirectX::XMFLOAT4>& points, std::vector<int>& indeces, float radius,
//		int segments, bool color);
//};


#pragma once

#include "BaseComponent.h"

class Game;

class SphereComponent : public BaseComponent
{
public:
    explicit SphereComponent(Game* game, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName);
    void SetPosition(DirectX::SimpleMath::Vector3 p) override { position = p; }
    void SetRotation(DirectX::SimpleMath::Quaternion q) override { rotation = q; }
    void SetScale(DirectX::SimpleMath::Vector3 s) override { scale = s; }
};
