#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {

    viewMatrix = Matrix::CreateLookAt(
        Vector3(0.0f,10.0f, -500.0f),
        Vector3::Zero,            
        Vector3::UnitY            
    );

    projMatrix = Matrix::CreatePerspectiveFieldOfView(
        DirectX::XM_PI / 4.f,
        float(game->Display->ClientWidth) / float(game->Display->ClientHeight),
        0.1f,
        10000.0f 
    );

    for (int i = 0; i < planetNum; ++i) {

        Planet* planet = new Planet(game);

        planet->diameter = i == 0 ? 1.0f : i + 1.0f;

        if (i % 3 == 0) {
            planet->mesh = new CubeComponent(game, planet->diameter);
        }
        else {
            planet->mesh = new SphereComponent(game, planet->diameter / 2, 5);
        }

        planet->mesh->Initialize();
        planet->mesh->viewMatrix = viewMatrix;
        planet->mesh->projMatrix = projMatrix;

        planet->orbitAngle = i * DirectX::XM_PI / 3.0f;
        planet->rotationSpeed = 0.8f - (i * 0.05f);
        planet->orbitSpeed = 6.0f / (i + 1);
        planet->orbitRadius = 20.0f + i * 12.0f;

        planets.push_back(planet);
    } 

    for (int i = 0; i < hasMoon.size(); ++i) {

        Moon* moon = new Moon(game);
        moon->parentPlanet = planets[hasMoon[i]];

        if (i % 2 == 0) {
            moon->mesh = new CubeComponent(game, planets[hasMoon[i]]->diameter / 3.0f);
            
        }
        else {
            moon->mesh = new SphereComponent(game, i == 0 ? 1.0f : i + 1.0f, 5);
        }

        moon->mesh->Initialize();
        moon->mesh->viewMatrix = viewMatrix;
        moon->mesh->projMatrix = projMatrix;

        moon->orbitRadius = moon->parentPlanet->diameter / 2.0f * 2.0f ;
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

    planets[0]->rotationSpeed = 0.5f;
    planets[0]->rotationAngle += planets[0]->rotationSpeed * game->DeltaTime;
    planets[0]->rotationAngle = fmodf(planets[0]->rotationAngle, DirectX::XM_2PI);
    planets[0]->mesh->worldMatrix = Matrix::CreateRotationY(planets[0]->rotationAngle);

    for (size_t i = 1; i < planets.size(); ++i) {
        planets[i]->Update(i);
    }

    for (auto moon : moons) {
        moon->Update();
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

void PlanetSystem::Planet::Update(size_t i) {

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

