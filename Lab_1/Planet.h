#pragma once
#include "Game.h"
#include "BaseComponent.h"
#include "SphereComponent.h"
#include "ObjectComponent.h"
#include "OrbitComponent.h"

class Planet {
public:
	void Update();

	Planet(Game* game) : game(game) {};

	Game* game;

	ObjectComponent* mesh;

	OrbitComponent* orbit;
	bool isOrbit = true;
	
	Vector3 pos = { 0.0f, 0.0f, 0.0f };

	float rotationAngle = 0.0f;
	float rotationSpeed = 0.0f;

	float orbitAngle = 0.0f;
	float orbitSpeed = 0.0f;
	float orbitRadius = 0.0f;

	float diameter;

	DirectX::SimpleMath::Quaternion rotation;
};

class Moon : public Planet {
public:
	void Update();

	Moon(Game* game) : Planet(game) {};

	Planet* parentPlanet;
	Vector3 relativePos = { 0.0f, 0.0f, 0.0f };
};

