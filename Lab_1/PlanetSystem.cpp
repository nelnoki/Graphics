#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {

    viewMatrix = Matrix::CreateLookAt(
        Vector3(1000.0f, 1000.0f, -1000.0f),
        Vector3::Zero,            
        Vector3::UnitZ            
    );

    projMatrix = Matrix::CreatePerspectiveFieldOfView(
        XM_PI / 4.f,
        float(game->Display->ClientWidth) / float(game->Display->ClientHeight),
        0.1f,
        10000.f 
    );

    for (int i = 0; i < 1; ++i) {
        Planet* planet = new Planet(
            game, 
            i % 9 == 0? 2 : 1,
            i == 0 ? 1.0f : i + 1.0f, 
            textures[i > 13 ? i - 13 * (i / 13) + 1 : i ]);

        planet->Initialize();
        planet->setViewMatrix(viewMatrix);
        planet->setProjMatrix(projMatrix);

        planet->orbitAngle = i * XM_PI / 3.0f;
        planet->rotationSpeed = 0.8f - (i * 0.05f);
        planet->orbitSpeed = 6.0f / (i + 1);
        planet->orbitRadius = 20.0f + i * 12.0f;

        planets.push_back(planet);
    } 

    //for (int i = 0; i < hasMoon.size(); ++i) {
    //    Moon* moon = new Moon(
    //        game, 
    //        i % 2 == 0 ? 2 : 1,
    //        planets[hasMoon[i]]->diameter / 3.0f,
    //        textures[i > 4? i - 5 * (i / 5) + 9 : 9 + i], 
    //        planets[hasMoon[i]]);

    //    moon->Initialize();
    //    moon->setViewMatrix(viewMatrix);
    //    moon->setProjMatrix(projMatrix);

    //    moon->orbitRadius = moon->parentPlanet->diameter / 2.0f * 2.0f ;
    //    moon->rotationSpeed = 3.0f + i;        
    //    moon->orbitSpeed = 10.0f - (i * 0.7f);

    //    moons.push_back(moon);
    //}
}

void PlanetSystem::Draw() {
    for (auto sphere : planets) {
        sphere->Draw();
    }

    for (auto moon : moons) {
        moon->Draw();
    }
}

void PlanetSystem::Update() {

    planets[0]->rotationSpeed = 0.5f;
    planets[0]->rotationAngle += planets[0]->rotationSpeed * game->DeltaTime;
    planets[0]->rotationAngle = fmodf(planets[0]->rotationAngle, XM_2PI);
    planets[0]->setWorldMatrix(Matrix::CreateRotationY(planets[0]->rotationAngle));

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
        i->DestroyResources();
    }
    for (auto i : moons) {
        i->DestroyResources();
    }
}

void PlanetSystem::Planet::Update(size_t i) {

    rotationAngle += rotationSpeed * game->DeltaTime;
    orbitAngle += orbitSpeed * game->DeltaTime;

    rotationAngle = fmodf(rotationAngle, XM_2PI);
    orbitAngle = fmodf(orbitAngle, XM_2PI);

    float x = orbitRadius * cos(orbitAngle);
    float z = orbitRadius * sin(orbitAngle);

    setWorldMatrix(
        Matrix::CreateTranslation(x, 0.0f, z) *
        Matrix::CreateRotationY(rotationAngle));
}

void PlanetSystem::Moon::Update() {

    rotationAngle += rotationSpeed * game->DeltaTime;
    rotationAngle = fmodf(rotationAngle, XM_2PI);

    orbitAngle += orbitSpeed * game->DeltaTime;
    orbitAngle = fmodf(orbitAngle, XM_2PI);

    float x = orbitRadius * cos(orbitAngle);
    float z = orbitRadius * sin(orbitAngle);

    setWorldMatrix(
        Matrix::CreateRotationY(rotationAngle) *
        Matrix::CreateTranslation(x, 0.0f, z) *
        parentPlanet->getWorldMatrix());
}