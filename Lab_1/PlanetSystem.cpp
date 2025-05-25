#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {

    viewMatrix = Matrix::CreateLookAt(
        Vector3(0.0f,10.0f, -500.0f),
        Vector3::Zero,            
        Vector3::UnitY            
    );

    projMatrix = Matrix::CreatePerspectiveFieldOfView(
        30.0f,
        float(game->Display->ClientWidth) / float(game->Display->ClientHeight),
        0.01f,
        100.0f 
    );

    for (int i = 0; i < planetNum; ++i) {

        Planet* planet = new Planet(game);

        planet->diameter = /*i == 0 ? 1.0f :*/ i + 0.1f;

        if (i % 5 == 0) {
            planet->mesh = new CubeComponent(game, planet->diameter);
        }
        else {
            planet->mesh = new SphereComponent(game, planet->diameter / 2, 50);
        }

        planet->mesh->Initialize();
        planet->mesh->viewMatrix = viewMatrix;
        planet->mesh->projMatrix = projMatrix;

        planet->orbitAngle = i * DirectX::XM_PI / 3.0f;
        planet->rotationSpeed = 0.08f - (i * 0.05f);
        planet->orbitSpeed = 6.0f / (i + 1);
        planet->orbitRadius = 20.0f + i * 5.0f;

        planets.push_back(planet);
    } 

    for (int i = 0; i < hasMoon.size(); ++i) {

        Moon* moon = new Moon(game);
        moon->parentPlanet = planets[hasMoon[i]];

        if (i % 2 == 0) {
            moon->mesh = new CubeComponent(game, planets[hasMoon[i]]->diameter / 3.0f);
            
        }
        else {
            moon->mesh = new SphereComponent(game, i == 0 ? 1.0f : i + 1.0f, 50);
        }

        moon->mesh->Initialize();
        moon->mesh->viewMatrix = viewMatrix;
        moon->mesh->projMatrix = projMatrix;

        moon->orbitRadius = moon->parentPlanet->diameter  * 2.0f ;
        moon->rotationSpeed = 3.0f + i;        
        moon->orbitSpeed = 10.0f - (i * 0.7f);

        moons.push_back(moon);
    }
}

void PlanetSystem::Draw() {
    for (auto planet : planets) {
        planet->mesh->Draw();
    }

    for (auto moon : moons) {
        moon->mesh->Draw();
    }
}

void PlanetSystem::Update() {
    HandleCameraInput();

    // 1. Обновляем позицию камеры
    Vector3 camPos = Vector3(0, 0, -cameraDistance); // Начальная позиция (по оси Z)
    Matrix rotMat = Matrix::CreateFromYawPitchRoll(cameraYaw, cameraPitch, cameraRoll);
    camPos = Vector3::Transform(camPos, rotMat);
        
    cameraLookAt = cameraLookAt + Vector3(0, 0.1f, 0);

    viewMatrix = Matrix::CreateLookAt(camPos, cameraLookAt, Vector3::Up);

    // 2. Обновляем viewMatrix
    //viewMatrix = Matrix::CreateLookAt(camPos, cameraLookAt, Vector3::Up);

  /*  planets[0]->rotationSpeed = 0.5f;
    planets[0]->rotationAngle += planets[0]->rotationSpeed * game->DeltaTime;
    planets[0]->rotationAngle = fmodf(planets[0]->rotationAngle, DirectX::XM_2PI);
    planets[0]->mesh->worldMatrix = Matrix::CreateRotationY(planets[0]->rotationAngle);*/

    for (auto planet : planets) {
        planet->mesh->viewMatrix = viewMatrix;
        planet->mesh->projMatrix = projMatrix;
        planet->Update();   
    }

    for (auto moon : moons) {
        moon->mesh->viewMatrix = viewMatrix;
        moon->mesh->projMatrix = projMatrix;
        moon->Update();
    }

    std::cout << "" << ":\n";
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            std::cout << viewMatrix(row, col) << "\t";
        }
        std::cout << "\n";
    }
}

void PlanetSystem::Reload() {}

void PlanetSystem::DestroyResources() {
    for (auto i : planets) {
        i->mesh->DestroyResources();
    }
    for (auto i : moons) {
        i->mesh->DestroyResources();
    }
}

void PlanetSystem::Planet::Update() {

    rotationAngle += rotationSpeed * game->DeltaTime;
    orbitAngle += orbitSpeed * game->DeltaTime;

    rotationAngle = fmodf(rotationAngle, DirectX::XM_2PI);
    orbitAngle = fmodf(orbitAngle, DirectX::XM_2PI);

    float x = orbitRadius * cos(orbitAngle);
    float z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix = 
        Matrix::CreateTranslation(x, 0.0f, z) *
        Matrix::CreateRotationY(rotationAngle);
}

void PlanetSystem::Moon::Update() {

    rotationAngle += rotationSpeed * game->DeltaTime;
    rotationAngle = fmodf(rotationAngle, DirectX::XM_2PI);

    orbitAngle += orbitSpeed * game->DeltaTime;
    orbitAngle = fmodf(orbitAngle, DirectX::XM_2PI);

    float x = orbitRadius * cos(orbitAngle);
    float z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix = 
        Matrix::CreateRotationY(rotationAngle) *
        Matrix::CreateTranslation(x, 0.0f, z) *
        parentPlanet->mesh->worldMatrix;
}

void PlanetSystem::RotateCamera(float deltaX, float deltaY) {
    cameraYaw += deltaX * cameraSensitivity;
    cameraPitch -= deltaY * cameraSensitivity; // Инвертируем вертикальное движение

    // Ограничиваем угол pitch
    const float maxPitch = DirectX::XM_PIDIV2 - 0.1f;
    const float minPitch = -DirectX::XM_PIDIV2 + 0.1f;

    if (cameraPitch > maxPitch) cameraPitch = maxPitch;
    if (cameraPitch < minPitch) cameraPitch = minPitch;
}

void PlanetSystem::ZoomCamera(float delta) {
    cameraDistance -= delta * zoomSpeed;

    // Ограничиваем дистанцию
    const float minDistance = 5.0f;
    const float maxDistance = 500.0f;

    if (cameraDistance < minDistance) cameraDistance = minDistance;
    if (cameraDistance > maxDistance) cameraDistance = maxDistance;
}

void PlanetSystem::HandleCameraInput() {
    // Получаем текущее положение мыши
    Vector2 currentMousePos = game->InDevice->MousePosition;
    int wheelDelta = game->InDevice->MouseWheelDelta;

    // Вращение при зажатой правой кнопке мыши
    if (game->InDevice->IsKeyDown(Keys::RightButton)) {
        Vector2 mouseDelta = currentMousePos - prevMousePos;
        RotateCamera(mouseDelta.x, mouseDelta.y);
        std::cout << "rightbutton is working \n";
    }

    // Зум колесом мыши
    if (wheelDelta != 0) {
        ZoomCamera(wheelDelta / 120.0f);
        std::cout << "wheel is working \n"; 
    }

    prevMousePos = currentMousePos;
}

