#include "Obj3DComponent.h"
#include "Game.h"
#include <WICTextureLoader.h>

Obj3DComponent::Obj3DComponent(
    Game* game,
    int typeObj,
    float diameter,
    LPCWSTR texture,
    size_t tessellation,
    DXGI_FORMAT indexFormat,
    LPCWSTR shader)
    : GameComponent(game),
    typeObj(typeObj),
    diameter(diameter),
    tessellation(tessellation),
    shaderSource(shader),
    textureSource(texture),
    indexFormat(indexFormat) 
    {
    worldMatrix = DirectX::SimpleMath::Matrix::Identity;

    viewMatrix = Matrix::CreateLookAt(
        Vector3(0.f, 2.f, -7.f),
        Vector3::Zero, 
        Vector3::UnitY);

    projMatrix = Matrix::CreatePerspectiveFieldOfView(
        XM_PI / 4.f,
        float(game->Display->ClientWidth) / float(game->Display->ClientHeight), 
        0.1f, 
        10.f);
    }

void Obj3DComponent::Initialize() {

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

    ID3DBlob* errorPixelCode;

    res = D3DCompileFromFile(
        shaderSource,
        nullptr /*macros*/,
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
        nullptr,
        &vertexShader);

    game->Device->CreatePixelShader(
        pixelShaderByteCode->GetBufferPointer(),
        pixelShaderByteCode->GetBufferSize(),
        nullptr,
        &pixelShader);


    D3D11_INPUT_ELEMENT_DESC inputElements[] = {
        D3D11_INPUT_ELEMENT_DESC { 
            "POSITION", 
            0, 
            DXGI_FORMAT_R32G32B32A32_FLOAT, 
            0, 
            0, 
            D3D11_INPUT_PER_VERTEX_DATA, 
            0 },
        D3D11_INPUT_ELEMENT_DESC { 
            "COLOR", 
            0, 
            DXGI_FORMAT_R32G32B32A32_FLOAT, 
            0, 
            D3D11_APPEND_ALIGNED_ELEMENT, 
            D3D11_INPUT_PER_VERTEX_DATA, 
            0 }
    };

    game->Device->CreateInputLayout(
        inputElements,
        2,
        vertexShaderByteCode->GetBufferPointer(),
        vertexShaderByteCode->GetBufferSize(),
        &layout);


    CD3D11_BUFFER_DESC cbDesc(
        sizeof(Matrix),
        D3D11_BIND_CONSTANT_BUFFER);

    game->Device->CreateBuffer(
        &cbDesc,
        nullptr,
        &constantBuffer);

    switch (typeObj) {
        case 1:
        {
            object = GeometricPrimitive::CreateSphere(
                game->Context,
                diameter,
                tessellation,
                false,
                indexFormat);
            break;
        }
        case 2:
        {
            object = GeometricPrimitive::CreateCube(
                game->Context,
                diameter,
                false);
            break;
        }
        default: 
        {
            object = GeometricPrimitive::CreateSphere(
                game->Context,
                diameter,
                tessellation,
                false,
                indexFormat);
            break;
        }
    }
    
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        game->Device.Get(),
        textureSource,
        nullptr,
        textureMatrix.ReleaseAndGetAddressOf()
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to load texture");
    }
}

void Obj3DComponent::Draw() {

    game->Context->IASetInputLayout(layout);
    game->Context->VSSetShader(vertexShader, nullptr, 0);
    game->Context->PSSetShader(pixelShader, nullptr, 0);

    Matrix viewProjMatrix = viewMatrix * projMatrix;

    game->Context->UpdateSubresource(
        constantBuffer,
        0,
        nullptr,
        &viewProjMatrix,
        0,
        0);

    game->Context->VSSetConstantBuffers(0, 1, &constantBuffer);

    object->Draw(worldMatrix, viewMatrix, projMatrix, Colors::White, textureMatrix.Get());
}

void Obj3DComponent::DestroyResources() {
    object.reset();
    textureMatrix.Reset();

    layout->Release();

    vertexShader->Release();
    vertexShaderByteCode->Release();

    pixelShader->Release();
    pixelShaderByteCode->Release();

    constantBuffer->Release();
}

void Obj3DComponent::Update() {}
void Obj3DComponent::Reload() {}