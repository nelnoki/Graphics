#include "OrbitComponent.h"
#include "Game.h"
#include "Tools/DataProcesser.h"
#include <d3dcompiler.h>

using namespace DirectX;
using namespace SimpleMath;
using namespace Microsoft::WRL;

OrbitComponent::OrbitComponent(
    Game* game, 
    const Vector3& center,
    float radius, 
    int segments,
    const Vector4& color)
    : GameComponent(game), center_(center), color_(color),
    radius_(radius), segments_(segments), vertexCount_(0)
{
    if (segments < 3) segments_ = 3;
    GenerateOrbitPoints();
}

OrbitComponent::~OrbitComponent()
{
    DestroyResources();
}

void OrbitComponent::GenerateOrbitPoints()
{
    vertices_.clear();
    const float angleStep = XM_2PI / segments_;

    for (int i = 0; i <= segments_; ++i)
    {
        float angle1 = i * angleStep;
        float angle2 = ((i + 1) % segments_) * angleStep;

        Vertex v1, v2;

        v1.position = Vector4(
            radius_ * cos(angle1), 
            0.0f, 
            radius_ * sin(angle1), 
            1.0f);
        /*v1.position += Vector4(
            center_.x, 
            center_.y, 
            center_.z, 
            0.0f);*/
        v1.color = color_;

        v2.position = Vector4(
            radius_ * cos(angle2), 
            0.0f, 
            radius_ * sin(angle2), 
            1.0f);
        /*v2.position += Vector4(
            center_.x, 
            center_.y, 
            center_.z, 
            0.0f);*/
        v2.color = color_;

        vertices_.push_back(v1);
        vertices_.push_back(v2);
    }
    vertexCount_ = vertices_.size();
}

void OrbitComponent::Initialize()
{
    ComPtr<ID3DBlob> errorBlob;
    HRESULT hr = D3DCompileFromFile(
        L"Shaders/OrbitShader.hlsl",
        nullptr,
        nullptr,
        "VSMain",
        "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &vsBlob_,
        &errorBlob);

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        throw std::runtime_error("Failed to compile vertex shader");
    }

    hr = game->Device->CreateVertexShader(
        vsBlob_->GetBufferPointer(),
        vsBlob_->GetBufferSize(),
        nullptr,
        &vertexShader_);

    hr = D3DCompileFromFile(
        L"Shaders/OrbitShader.hlsl",
        nullptr,
        nullptr,
        "PSMain",
        "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &psBlob_,
        &errorBlob);

    if (FAILED(hr)) {
        if (errorBlob) {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
        }
        throw std::runtime_error("Failed to compile pixel shader");
    }

    hr = game->Device->CreatePixelShader(
        psBlob_->GetBufferPointer(),
        psBlob_->GetBufferSize(),
        nullptr,
        &pixelShader_);

    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = game->Device->CreateInputLayout(
        inputElements,
        2,
        vsBlob_->GetBufferPointer(),
        vsBlob_->GetBufferSize(),
        &layout_);

    D3D11_BUFFER_DESC vertexBufDesc = {};
    vertexBufDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufDesc.ByteWidth = sizeof(Vertex) * vertices_.size();

    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices_.data();

    hr = game->Device->CreateBuffer(&vertexBufDesc, &vertexData, &vertexBuffer_);

    D3D11_BUFFER_DESC constBufDesc = {};
    constBufDesc.ByteWidth = sizeof(Matrix);
    constBufDesc.Usage = D3D11_USAGE_DEFAULT;
    constBufDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = game->Device->CreateBuffer(&constBufDesc, nullptr, &constantBuffer_);
}

void OrbitComponent::Draw()
{
    Matrix worldViewProj =  
        Matrix::CreateTranslation(center_) *
        game->Camera->GetViewMatrix() *
        game->Camera->GetProjMatrix();

    game->Context->UpdateSubresource(
        constantBuffer_.Get(),
        0,
        nullptr,
        &worldViewProj,
        0,
        0
    );

    game->Context->VSSetShader(vertexShader_.Get(), nullptr, 0);
    game->Context->PSSetShader(pixelShader_.Get(), nullptr, 0);
    game->Context->IASetInputLayout(layout_.Get());
    game->Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    game->Context->VSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    game->Context->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);

    game->Context->Draw(vertexCount_, 0);
}

void OrbitComponent::SetCenter(const Vector3& center)
{
    if (center_ != center) {
        center_ = center;

        /*D3D11_MAPPED_SUBRESOURCE mappedResource;
        HRESULT hr = game->Context->Map(
            vertexBuffer_.Get(),
            0,
            D3D11_MAP_WRITE_DISCARD,
            0,
            &mappedResource);

        if (SUCCEEDED(hr)) {
            Vertex* vertices = static_cast<Vertex*>(mappedResource.pData);
            const float angleStep = XM_2PI / segments_;

            for (int i = 0; i <= segments_; ++i) {
                float angle1 = i * angleStep;
                float angle2 = ((i + 1) % segments_) * angleStep;

                vertices[2 * i].position = Vector4(
                    radius_ * cos(angle1),
                    0.0f,
                    radius_ * sin(angle1),
                    1.0f
                ) + Vector4(center_.x, center_.y, center_.z, 0.0f);

                vertices[2 * i + 1].position = Vector4(
                    radius_ * cos(angle2),
                    0.0f,
                    radius_ * sin(angle2),
                    1.0f
                ) + Vector4(center_.x, center_.y, center_.z, 0.0f);
            }

            game->Context->Unmap(vertexBuffer_.Get(), 0);
        }*/
    }
}

void OrbitComponent::Update() {}

void OrbitComponent::DestroyResources()
{
    vertexBuffer_->Release();
    vertexBuffer_ = nullptr;
    layout_->Release();
    layout_ = nullptr;
}