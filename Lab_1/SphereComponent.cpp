#include "SphereComponent.h"
#include <cmath>

SphereComponent::SphereComponent(
    Game* game,
    float radius,
    int segments,
    std::vector<UINT> strides,
    std::vector<UINT> offsets,
    LPCWSTR shader) : TriangleComponent(game) {

    std::vector<DirectX::XMFLOAT4> points;
    std::vector<int> indeces;

    GenerateSpherePoints(points, indeces, radius, segments);

    // Инициализируем базовый класс с полученными вершинами и индексами
    this->points = points;
    this->indeces = indeces;
    this->strides = { 32 };
    this->offsets = { 0 };
    this->shaderSource = shader;
}

void SphereComponent::GenerateSpherePoints(std::vector<DirectX::XMFLOAT4>& points, std::vector<int>& indeces, float radius,
    int segments) {

    points.clear();
    indeces.clear();

    for (int i = 0; i <= segments; ++i) {
        float v = static_cast<float>(i) / segments;
        float theta = v * DirectX::XM_PI;

        for (int j = 0; j <= segments; ++j) {
            float u = static_cast<float>(j) / segments;
            float phi = u * 2.0f * DirectX::XM_PI;

            float x = radius * sinf(theta) * cosf(phi);
            float y = radius * cosf(theta);
            float z = radius * sinf(theta) * sinf(phi);

            points.push_back(DirectX::XMFLOAT4(x, y, z, 1.0f));
            points.push_back(DirectX::XMFLOAT4(u, v, 0.5f, 1.0f)); 
        }
    }

    for (int i = 0; i < segments; ++i) {
        for (int j = 0; j < segments; ++j) {
            int first = (i * (segments + 1)) + j;
            int second = first + segments + 1;

            indeces.push_back(first);
            indeces.push_back(second);
            indeces.push_back(first + 1);

            indeces.push_back(second);
            indeces.push_back(second + 1);
            indeces.push_back(first + 1);
        }
    }
}