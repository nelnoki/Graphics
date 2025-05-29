#pragma once
#include <wrl.h>
#include <d3d.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <SimpleMath.h>

#include <unordered_set>
#include "Keys.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using namespace DirectX::SimpleMath;

#include "GameComponent.h"

class CameraComponent : public GameComponent {
public:

    DirectX::SimpleMath::Matrix viewMatrix;
    DirectX::SimpleMath::Matrix projMatrix;

    Vector3 lookPoint;
    Vector3 target;
    Vector3 upAxis;

    CameraComponent(Game* game) :
        GameComponent(game), 
        viewMatrix(DirectX::SimpleMath::Matrix::Identity),
        projMatrix(DirectX::SimpleMath::Matrix::Identity) 
    {};

    void Initialize() override = 0;
    void Draw() override = 0;
    void Reload() override = 0;
    void Update() override = 0;
    void DestroyResources() override = 0;

    virtual void HandleInput(float deltaTime) {};
    virtual void Rotate(Vector2 mouseInput) {};
    virtual void Move(float deltaTime) {};
    virtual void Zoom(float amount) {};

    
};

