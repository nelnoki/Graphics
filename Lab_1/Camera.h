#pragma once
#include <SimpleMath.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

class Camera
{
protected:
    DirectX::SimpleMath::Matrix viewMatrix;
    DirectX::SimpleMath::Matrix projMatrix;

public:
    bool IsOrthographic;
    float OrthographicWidth;
    float OrthographicHeight;
    float FOV;
    float AspectRatio;
    float NearPlane;
    float FarPlane;
    DirectX::SimpleMath::Vector3 Position;
    DirectX::SimpleMath::Vector3 Target;
    DirectX::SimpleMath::Vector3 Up;
    Camera();
    void Update();
    DirectX::SimpleMath::Matrix GetViewMatrix() const;
    DirectX::SimpleMath::Matrix GetProjMatrix() const;
};
