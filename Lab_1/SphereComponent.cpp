//#include "SphereComponent.h"
//#include <cmath>
//
//SphereComponent::SphereComponent(
//    Game* game,
//    float radius,
//    int segments,
//    bool color,
//    std::vector<UINT> strides,
//    std::vector<UINT> offsets,
//    LPCWSTR shader) : TriangleComponent(game) {
//
//    std::vector<DirectX::XMFLOAT4> points;
//    std::vector<int> indeces;
//
//    GenerateSpherePoints(points, indeces, radius, segments, color);
//
//    // Инициализируем базовый класс с полученными вершинами и индексами
//    this->points = points;
//    this->indeces = indeces;
//    this->strides = { 32 };
//    this->offsets = { 0 };
//    this->shaderSource = shader;
//}
//
//void SphereComponent::GenerateSpherePoints(std::vector<DirectX::XMFLOAT4>& points, std::vector<int>& indeces, float radius,
//    int segments, bool color) {
//
//    points.clear();
//    indeces.clear();
//
//    for (int i = 0; i <= segments; ++i) {
//        float v = static_cast<float>(i) / segments;
//        float theta = v * DirectX::XM_PI;
//
//        for (int j = 0; j <= segments; ++j) {
//            float u = static_cast<float>(j) / segments;
//            float phi = u * 2.0f * DirectX::XM_PI;
//
//            float x = radius * sinf(theta) * cosf(phi);
//            float y = radius * cosf(theta);
//            float z = radius * sinf(theta) * sinf(phi);
//
//            points.push_back(DirectX::XMFLOAT4(x, y, z, 1.0f));
//
//            if (color)
//                points.push_back(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
//            else
//                points.push_back(DirectX::XMFLOAT4(u, v, 0.5f, 1.0f)); 
//        }
//    }
//
//    for (int i = 0; i < segments; ++i) {
//        for (int j = 0; j < segments; ++j) {
//            int first = (i * (segments + 1)) + j;
//            int second = first + segments + 1;
//
//            indeces.push_back(first);
//            indeces.push_back(second);
//            indeces.push_back(first + 1);
//
//            indeces.push_back(second);
//            indeces.push_back(second + 1);
//            indeces.push_back(first + 1);
//        }
//    }
//}

#include "SphereComponent.h"
#include "Game.h"

using namespace DirectX;
using namespace SimpleMath;

SphereComponent::SphereComponent(Game* g, float radius, int sliceCount, int stackCount, const wchar_t* diffuseTextureName)
	: BaseComponent(g)
{
	textureFileName_ = diffuseTextureName;

	Vertex topPoint({ Vector4(0.0f, radius, 0.0f, 1.0f), Vector4(0.0f, 0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f) });
	Vertex bottomPoint({ Vector4(0.0f, -radius, 0.0f, 1.0f), Vector4(0.0f, 1.0f, 0.0f, 0.0f), Vector4(0.0f, -1.0f, 0.0f, 0.0f) });

	points_.push_back(topPoint);

	const float phiStep = XM_PI / static_cast<float>(stackCount);
	const float thetaStep = XM_2PI / static_cast<float>(sliceCount);

	for (int i = 1; i <= stackCount - 1; ++i)
	{
		const float phi = static_cast<float>(i) * phiStep;

		for (int j = 0; j <= sliceCount; ++j)
		{
			const float theta = static_cast<float>(j) * thetaStep;
			Vertex p;
			p.pos.x = radius * sinf(phi) * cosf(theta);
			p.pos.y = radius * cosf(phi);
			p.pos.z = radius * sinf(phi) * sinf(theta);
			p.pos.w = 1.0f;
			p.tex.x = theta / XM_2PI;
			p.tex.y = phi / XM_PI;
			p.normal = p.pos;
			p.normal.w = 0.0f;
			p.normal.Normalize();
			points_.push_back(p);
		}
	}

	points_.push_back(bottomPoint);

	for (int i = 1; i <= sliceCount; ++i)
	{
		indices_.push_back(0);
		indices_.push_back(i + 1);
		indices_.push_back(i);
	}

	int baseIndex = 1;
	const int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount - 2; ++i)
	{
		for (int j = 0; j < sliceCount; ++j)
		{
			indices_.push_back(baseIndex + i * ringVertexCount + j);
			indices_.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices_.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices_.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	const int southPoleIndex = points_.size() - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < sliceCount; ++i)
	{
		indices_.push_back(southPoleIndex);
		indices_.push_back(baseIndex + i);
		indices_.push_back(baseIndex + i + 1);
	}
}