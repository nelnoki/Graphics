#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {

    Vector3 camOrbit = Vector3(-2.0f, 5.0f, -2.0f);

    for (int i = 0; i < planetNum; ++i) {

        Planet* planet = new Planet(game);

        planet->diameter = i + 0.1f;

        if (i % 3 == 0) {
            planet->mesh = new CubeComponent(game, planet->diameter);
        }
        else {
            planet->mesh = new SphereComponent(game, planet->diameter / 3, 50);
        }

        if (i % 4 == 0)
            hasMoon.push_back(i);

        planet->mesh->Initialize();

        planet->orbitAngle = i * DirectX::XM_PI / 3.0f;
        planet->rotationSpeed = 0.08f + (i * 0.005f);
        planet->orbitSpeed = 6.0f / (i + 15.0f);
        planet->orbitRadius = 30.0f + i * 5.0f;
        planet->pos.x = planet->orbitRadius * cos(planet->orbitAngle);
        planet->pos.z = planet->orbitRadius * sin(planet->orbitAngle);

        planets.push_back(planet);

        OrbitalCamera* cam = new OrbitalCamera(game);
        cam->Initialize(camOrbit, planet->pos - camOrbit, planet->pos);
        OrbitalCameraObject* camObject = new OrbitalCameraObject{ cam, planet };
        cameras.push_back(camObject);
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

        moon->orbitRadius = moon->parentPlanet->diameter  * 2.0f ;
        moon->rotationSpeed = 3.0f + i;        
        moon->orbitSpeed = 10.0f / (i + 5.0f);

        moons.push_back(moon);
    }

    mainFPS = new FPSCamera(game);
    mainFPS->Initialize();
    mainFPS->SetLookPoint(Vector3(3.0f, -3.0f, 3.0f));
    mainFPS->SetTarget(Vector3(0.0f, 0.0f, 0.0f));

    activeCam = mainFPS;
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

    /*for (OrbitalCameraObject* cam : cameras) {
        cam->camera->SetTarget(cam->cameraOwner->pos);
        cam->camera->SetLookPoint(
            cam->cameraOwner->pos + 
            Vector3::Transform(
                cam->camera->orbit, 
                Matrix::CreateRotationY(cam->cameraOwner->orbitAngle)));
    }*/

    for (auto planet : planets) planet->Update();
    for (auto moon : moons) moon->Update();

    for (OrbitalCameraObject* cam : cameras) {
        cam->camera->SetTarget(cam->cameraOwner->pos);
    }

    activeCam->HandleInput(game->DeltaTime);
    activeCam->Update();

    for (auto planet : planets) {
        planet->mesh->viewMatrix = activeCam->viewMatrix;
        planet->mesh->projMatrix = activeCam->projMatrix;
    }

    for (auto moon : moons) {
        moon->mesh->viewMatrix = activeCam->viewMatrix;
        moon->mesh->projMatrix = activeCam->projMatrix;
    }

    for (int i = 0; i <= cameras.size(); i++) {
        Keys key = static_cast<Keys>(static_cast<int>(Keys::D0) + i);
        if (game->InDevice->IsKeyDown(key)) {
            ChangeCamera(i - 1);
        }
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

    pos.x = orbitRadius * cos(orbitAngle);
    pos.z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix = 
        Matrix::CreateTranslation(pos) *
        Matrix::CreateRotationY(rotationAngle);
}

void PlanetSystem::Moon::Update() {

    rotationAngle += rotationSpeed * game->DeltaTime;
    rotationAngle = fmodf(rotationAngle, DirectX::XM_2PI);

    orbitAngle += orbitSpeed * game->DeltaTime;
    orbitAngle = fmodf(orbitAngle, DirectX::XM_2PI);

    pos.x = orbitRadius * cos(orbitAngle);
    pos.z = orbitRadius * sin(orbitAngle);

    mesh->worldMatrix = 
        Matrix::CreateRotationY(rotationAngle) *
        Matrix::CreateTranslation(pos) *
        parentPlanet->mesh->worldMatrix;
}

void PlanetSystem::ChangeCamera(int cameraID)
{
    if (cameraID == -1) 
        activeCam = mainFPS;
    else 
        activeCam = cameras[cameraID]->camera;
}