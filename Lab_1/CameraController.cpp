#include "CameraController.h"
#include "Game.h"
#include "PlanetSystem.h"
#include "Planet.h"

using namespace DirectX;
using namespace SimpleMath;

CameraController::CameraController(PlanetSystem* PS) 
    : targetBody(nullptr), 
    up(Vector3::Up), 
    game(PS->game), 
    OrbitMode(true), 
    speed(0.2f), 
    planetSystem(PS)
{
    relativePos = game->Camera->Position;
}

void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{

    if (OrbitMode)
    {
        if (game->InDevice->IsKeyDown(Keys::LeftButton))
        {
            auto right = relativePos.Cross(up);
            auto qua = Quaternion::CreateFromAxisAngle(up, 0.005f * game->InDevice->MouseOffset.x) 
                * Quaternion::CreateFromAxisAngle(right, -0.005f * game->InDevice->MouseOffset.y);
            relativePos = XMVector3Transform(relativePos, Matrix::CreateFromQuaternion(qua));
            up = XMVector3Transform(Vector3::Up, Matrix::CreateFromQuaternion(qua));
            up = Vector3::Up;
            game->Camera->Up = up;
        }
     
        relativePos *= 1 - 0.001f * game->InDevice->MouseWheelDelta;

    }
    else
    {
        if (game->InDevice->IsKeyDown(Keys::LeftButton))
        {
            yaw -= 0.004f * game->InDevice->MouseOffset.x;
            while (yaw < -XM_2PI)
                yaw += XM_2PI;
            pitch -= 0.004f * game->InDevice->MouseOffset.y;
            game->Camera->Up = XMVector4Transform(
                Vector3::Up, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            game->Camera->Target = game->Camera->Position;
            game->Camera->Target += XMVector4Transform(
                Vector3::Forward, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        }
    }
}

void CameraController::Update()
{
    if (OrbitMode && (
        game->InDevice->IsKeyDown(Keys::W) || 
        game->InDevice->IsKeyDown(Keys::A) || 
        game->InDevice->IsKeyDown(Keys::S) || 
        game->InDevice->IsKeyDown(Keys::D) ||
        game->InDevice->IsKeyDown(Keys::E) || 
        game->InDevice->IsKeyDown(Keys::Q)))
    {
        OrbitMode = false;
        auto q = Quaternion::LookRotation(
            game->Camera->Target - game->Camera->Position, 
            game->Camera->Up);
        auto v = q.ToEuler();
        yaw = v.y;
        pitch = 0.0f;
        targetBody = nullptr;
    }
    if (game->InDevice->IsKeyDown(Keys::D0))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[0];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    if (game->InDevice->IsKeyDown(Keys::D1))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[1];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    if (game->InDevice->IsKeyDown(Keys::D2))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[2];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    if (game->InDevice->IsKeyDown(Keys::D3))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[3];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    if (game->InDevice->IsKeyDown(Keys::D4))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[4];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }
    if (game->InDevice->IsKeyDown(Keys::D5))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[5];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }

    if (game->InDevice->IsKeyDown(Keys::D6))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[6];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }

    if (game->InDevice->IsKeyDown(Keys::D7))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[7];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }

    if (game->InDevice->IsKeyDown(Keys::D8))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[9];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }

    if (game->InDevice->IsKeyDown(Keys::D9))
    {
        OrbitMode = true;
        targetBody = planetSystem->planets[9];
        game->Camera->Position = Vector3::Forward * 20.0f;
        game->Camera->Up = Vector3::Up;
        up = Vector3::Up;
    }

    if (OrbitMode && targetBody)
    {
        game->Camera->Target = targetBody->pos;
        game->Camera->Position = targetBody->pos + relativePos;
    }

    if (OrbitMode && !targetBody)
    {
        game->Camera->Target = Vector3::Zero;
        game->Camera->Position = relativePos;
    }

    if (!OrbitMode)
    {
        if (game->InDevice->IsKeyDown(Keys::W))
        {
            Vector3 tmp = XMVector4Transform(
                Vector3::Forward, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->InDevice->IsKeyDown(Keys::S))
        {
            Vector3 tmp = XMVector4Transform(
                Vector3::Backward, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->InDevice->IsKeyDown(Keys::A))
        {
            Vector3 tmp = XMVector4Transform(
                Vector3::Left, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->InDevice->IsKeyDown(Keys::D))
        {
            Vector3 tmp = XMVector4Transform(
                Vector3::Right, 
                Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
            tmp.Normalize();
            game->Camera->Position += speed * tmp;
        }
        if (game->InDevice->IsKeyDown(Keys::E))
        {
            game->Camera->Position += speed * Vector3::Up;
        }
        if (game->InDevice->IsKeyDown(Keys::Q))
        {
            game->Camera->Position += speed * Vector3::Down;
        }
        game->Camera->Up = XMVector4Transform(
            Vector3::Up, 
            Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
        game->Camera->Target = game->Camera->Position;
        game->Camera->Target += XMVector4Transform(
            Vector3::Forward, 
            Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f));
    }
}

