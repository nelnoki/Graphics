#pragma once
#include <wrl.h>
#include <DirectXHelpers.h>
#include <GeometricPrimitive.h>
#include <directxmath.h>
#include <SimpleMath.h>
#include "GameComponent.h"
#include "CubeComponent.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

class Obj3DComponent : public GameComponent {
public:
    void DestroyResources() override;
    void Draw() override;
    void Initialize() override;
    void Update() override;
    void Reload() override;

    Obj3DComponent(
        Game* game,
        int typeObj = 1,
        float diameter = 1.0f,
        LPCWSTR texture = L"./Textures/earth.bmp",
        size_t tessellation = 16,
        DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT,
        LPCWSTR shader = L"./Shaders/MyVeryFirstShader.hlsl");

    void setWorldMatrix(Matrix m) { object->worldMatrix = m; };
    void setViewMatrix(Matrix m) { object->viewMatrix = m; };
    void setProjMatrix(Matrix m) { object->projMatrix = m; };

    Matrix getWorldMatrix() { return object->worldMatrix; };
    Matrix getViewMatrix() { return object->viewMatrix; };
    Matrix getProjMatrix() { return object->projMatrix; };

   /* Matrix worldMatrix = Matrix::Identity;
    Matrix viewMatrix;
    Matrix projMatrix;*/

    //std::unique_ptr<GeometricPrimitive> object;
    TriangleComponent* object;
    int typeObj;
    LPCWSTR shaderSource;
    LPCWSTR textureSource;
    float diameter;
    size_t tessellation;
    DXGI_FORMAT indexFormat;

    ID3D11InputLayout* layout;

    ID3D11PixelShader* pixelShader;
    ID3DBlob* pixelShaderByteCode;

    ID3D11VertexShader* vertexShader;
    ID3DBlob* vertexShaderByteCode;

    ID3D11Buffer* constantBuffer;

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureMatrix;

    //ID3D11DepthStencilState* depthStencilState;
};