#include "PlanetSystem.h"
#include "Game.h"

void PlanetSystem::Initialize() {
    controller = new CameraController(this);
    game->InDevice->MouseMove.AddRaw(controller, &CameraController::OnMouseMove);

    Planet* sun = new Planet(game);
    sun->diameter = 10.0f; 

    sun->mesh = new ObjectComponent(game, "Models/sphere.obj", L"Textures/sun.dds");
    sun->mesh->SetScale(Vector3(0.1f, 0.1f, 0.1f));
    sun->mesh->SetPosition(Vector3(0.0f, 0.0f, 0.0f));

    sun->mesh->shininess = 10000000.0f;
    sun->mesh->specularColor = Vector3(5.0f, 1.0f, 0.0f);
    sun->mesh->diffuseColor = Vector3(5.0f, 1.0f, 0.0f);

    //sun->mesh = new SphereComponent(game, sun->diameter / 3, 32, 32, texture_0);

    sun->mesh->Initialize();

    sun->rotationSpeed = 0.3f;
    sun->orbitSpeed = 0.0f;
    sun->orbitRadius = .0f;
    sun->pos.x = 0.0f;
    sun->pos.z = 0.0f;

    sun->isOrbit = false;

    planets.push_back(sun);

    for (int i = 1; i < planetNum; ++i) {

        Planet* planet = new Planet(game);

        planet->diameter = i + 0.1f;

        const wchar_t* texture;
        switch (i) {
            case 1:
                texture = texture_1;
                break;
            case 2:
                texture = texture_2;
                break;
            case 3:
                texture = texture_3;
                break;
            case 4:
                texture = texture_4;
                break;
            case 5:
                texture = texture_5;
                break;
            case 6:
                texture = texture_6;
                break;
            case 7:
                texture = texture_7;
                break;
            case 8:
                texture = texture_8;
                break;
            case 9:
                texture = texture_9;
                break;
            default:
                texture = texture_3;
                break;
        }

        //if (i % 3 == 0) {
            //planet->mesh = new CubeComponent(game, planet->diameter);
        //}
        //else {
            //planet->mesh = new SphereComponent(game, planet->diameter / 3, 32, 32, texture);
        //}

        planet->mesh = new ObjectComponent(game, "Models/sphere.obj", texture);
        planet->mesh->SetScale(Vector3(0.01f * i, 0.01f * i, 0.01f * i));
        planet->mesh->SetPosition(Vector3(
                planet->orbitRadius * cos(planet->orbitAngle),
                0.0f,
                planet->orbitRadius * sin(planet->orbitAngle)));

        planet->mesh->shininess = 10000000.0f;
        planet->mesh->specularColor = Vector3(1.0f, 1.0f, 1.0f);
        planet->mesh->diffuseColor = Vector3(1.0f, 1.0f, 1.0f);

        planet->mesh->Initialize();

        planet->orbitAngle = i * DirectX::XM_PI / 3.0f;
        planet->rotationSpeed = 0.08f + (i * 0.9f);
        planet->orbitSpeed = 5.0f / (i * 3.0f);
        planet->orbitRadius = 30.0f + i * 10.0f;
        planet->pos.x = planet->orbitRadius * cos(planet->orbitAngle);
        planet->pos.z = planet->orbitRadius * sin(planet->orbitAngle);

        planet->orbit = new OrbitComponent(
            game,
            planets[0]->pos,
            planet->orbitRadius
        );

        planet->orbit->Initialize();

        planets.push_back(planet);
    } 

    for (int i = 0; i < hasMoon.size(); ++i) {

        Moon* moon = new Moon(game);
        moon->parentPlanet = planets[hasMoon[i]];

        const wchar_t* texture;
        Vector3 color;
        switch (i) {
        case 0:
            texture = texture_10;
            color = Vector3(1.0f, 1.0f, 10.0f);
            break;
        case 1:
            texture = texture_11;
            color = Vector3(1.0f, 10.0f, 1.0f);
            break;
        case 2:
            texture = texture_12;
            color = Vector3(10.0f, 1.0f, 1.0f);
            break;
        case 3:
            texture = texture_13;
            color = Vector3(10.0f, 10.0f, 1.0f);
            break;
        default:
            texture = texture_10;
            color = Vector3(1.0f, 1.0f, 1.0f);
            break;
        }

        //if (i % 2 == 0) {
            //moon->mesh = new CubeComponent(game, planets[hasMoon[i]]->diameter / 2.0f);
        //}
        //else {
            //moon->mesh = new SphereComponent(game, planets[hasMoon[i]]->diameter / 2.0f, 32, 32, texture);
        //}

        moon->mesh = new ObjectComponent(game, "Models/sphere.obj", texture);
        moon->mesh->SetScale(Vector3(0.008f * (i + 1), 0.008f * (i + 1), 0.008f * (i + 1)));
        
        moon->mesh->shininess = 10000000.0f;
        moon->mesh->specularColor = color;
        moon->mesh->diffuseColor = color;

        moon->mesh->Initialize();

        moon->orbitRadius = moon->parentPlanet->diameter  * 2.0f ;
        moon->rotationSpeed = 3.0f + i;        
        moon->orbitSpeed = 10.0f / (i + 5.0f);
        moon->relativePos = Vector3::Forward * moon->orbitRadius;
        moon->pos = moon->parentPlanet->pos + moon->relativePos;
        moon->mesh->SetPosition(moon->parentPlanet->pos + moon->relativePos);

        moon->orbit = new OrbitComponent(
            game,
            moon->parentPlanet->pos,
            moon->orbitRadius
        );

        moon->orbit->Initialize();

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

    for (auto planet : planets) {
        if (planet->isOrbit)
            planet->orbit->Draw();
    }

    for (auto moon : moons) {
        moon->orbit->Draw();
    }
}

void PlanetSystem::Update() {

    controller->Update();

    game->Camera->Update();
    for (auto planet : planets) {
        planet->Update();
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



