#pragma once
#include "GameComponent.h"
#include <vector>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include "Vertex.h"
#include <SimpleMath.h>
#include <wrl/client.h>

class OrbitComponent : public GameComponent {
public:
    struct Vertex {
        DirectX::SimpleMath::Vector4 position;
        DirectX::SimpleMath::Vector4 color;
    };

    OrbitComponent(
        Game* game, 
        const DirectX::SimpleMath::Vector3& center,
        float radius, 
        int segments = 64,
        const DirectX::SimpleMath::Vector4& color = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 1.0f));
    ~OrbitComponent();

    void Initialize() override;
    void DestroyResources() override;
    void Draw() override;
    void Update() override;
    void Reload() override {}

    void SetCenter(const DirectX::SimpleMath::Vector3& center);
    void SetRadius(float radius);

private:
    void GenerateOrbitPoints();

    std::vector<Vertex> vertices_;
    DirectX::SimpleMath::Vector3 center_;
    DirectX::SimpleMath::Vector4 color_;
    float radius_;
    int segments_;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer_;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout_;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader_;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader_;
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob_;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob_;
    Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer_;

    UINT vertexCount_;
};