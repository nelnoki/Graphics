#pragma once
#include "InputDevice.h"

class Game;
class PlanetSystem;
class Planet;

class CameraController {
public:
    Planet* targetBody;
    DirectX::SimpleMath::Vector3 up;
    DirectX::SimpleMath::Vector3 relativePos;
    Game* game;
    PlanetSystem* planetSystem;
    bool OrbitMode;

    float yaw;
    float pitch;
    float speed;

    explicit CameraController(PlanetSystem* PS);
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
};
