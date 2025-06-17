#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {
    controller = new CameraController(this);
    game->InDevice->MouseMove.AddRaw(controller, &CameraController::OnMouseMove);

    Planet* sun = new Planet(game);
    sun->diameter = 10.0f; 

    sun->mesh = new SphereComponent(game, sun->diameter / 3, 50, false);

    sun->mesh->Initialize();

    sun->rotationSpeed = 0.3f;
    
    planets.push_back(sun);

    for (int i = 1; i < planetNum; ++i) {

        Planet* planet = new Planet(game);

        planet->diameter = i + 0.1f;

        //if (i % 3 == 0) {
            //planet->mesh = new CubeComponent(game, planet->diameter);
        //}
        //else {
            planet->mesh = new SphereComponent(game, planet->diameter / 3, 50, i == 1? true : false);
        //}

        if (i % 3 == 0)
            hasMoon.push_back(i);

        planet->mesh->Initialize();

        planet->orbitAngle = i * DirectX::XM_PI / 3.0f;
        planet->rotationSpeed = 0.08f + (i * 0.005f);
        planet->orbitSpeed = 6.0f / (i + 15.0f);
        planet->orbitRadius = 30.0f + i * 5.0f;
        planet->pos.x = planet->orbitRadius * cos(planet->orbitAngle);
        planet->pos.z = planet->orbitRadius * sin(planet->orbitAngle);

        planets.push_back(planet);
    } 

    for (int i = 0; i < hasMoon.size(); ++i) {

        Moon* moon = new Moon(game);
        moon->parentPlanet = planets[hasMoon[i]];

        //if (i % 2 == 0) {
            moon->mesh = new CubeComponent(game, planets[hasMoon[i]]->diameter / 2.0f);
        //}
        //else {
            //moon->mesh = new SphereComponent(game, i == 0 ? 1.0f : i + 1.0f, 50);
        //}

        moon->mesh->Initialize();

        moon->orbitRadius = moon->parentPlanet->diameter  * 2.0f ;
        moon->rotationSpeed = 3.0f + i;        
        moon->orbitSpeed = 10.0f / (i + 5.0f);
        moon->relativePos = Vector3::Forward * moon->orbitRadius;
        moon->pos = moon->parentPlanet->pos + moon->relativePos;

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

    controller->Update();

    game->Camera->Update();

    for (auto planet : planets) planet->Update();
    for (auto moon : moons) moon->Update();
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



