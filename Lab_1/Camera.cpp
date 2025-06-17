#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

Camera::Camera()
{
    viewMatrix = Matrix::Identity;
    projMatrix = Matrix::Identity;

    FOV = XM_PI / 4.0f;
    AspectRatio = 1.0f;
    NearPlane = 1.0f;
    FarPlane = 1000.f;
    Position = Vector3::Backward * 30.0f;
    Target = Vector3::Zero;
    Up = Vector3::Up;
    IsOrthographic = false;
    OrthographicWidth = 50.0f;
    OrthographicHeight = 50.0f;
}

void Camera::Update()
{
    viewMatrix = Matrix::CreateLookAt(Position, Target, Up);
    projMatrix = Matrix::CreatePerspectiveFieldOfView(FOV, AspectRatio, NearPlane, FarPlane);
}

Matrix Camera::GetViewMatrix() const
{
    return viewMatrix;
}

Matrix Camera::GetProjMatrix() const
{
    return projMatrix;
}

