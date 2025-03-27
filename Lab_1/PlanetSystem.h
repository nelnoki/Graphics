#pragma once
#include "GameComponent.h"
#include "Obj3DComponent.h"

class PlanetSystem : public GameComponent {
public:
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;

	PlanetSystem(
		Game* game, 
		int planetNumber = 30, 
		std::vector<int> hasMoon = { 1,2,3,6,7,8,9,11,12,13,14,18,19,20,26,27,28,29 } ) 
			: GameComponent(game), planetNum(planetNumber), hasMoon(hasMoon){};

	std::vector<int> hasMoon;
	int planetNum;
	std::vector<LPCWSTR> textures = { 
		L"./Textures/sun.jpg",
		L"./Textures/mercury.jpg", 
		L"./Textures/venus.jpg", 
		L"./Textures/earth.jpg", 
		L"./Textures/mars.jpg",
		L"./Textures/jupiter.jpg",
		L"./Textures/saturn.jpg",
		L"./Textures/uranus.jpg",
		L"./Textures/neptune.jpg",
		L"./Textures/moon.jpg",
		L"./Textures/ceres.jpg",
		L"./Textures/eris.jpg",
		L"./Textures/haumea.jpg",
		L"./Textures/makemake.jpg" };

	Matrix viewMatrix;
	Matrix projMatrix;

	class Planet : public Obj3DComponent {
	public:
		void Update(size_t i);

		Planet(Game* game,
			int type,
			float diameter,
			LPCWSTR texture)
			: Obj3DComponent(game, type, diameter, texture) {}

		float rotationAngle = 0.0f;
		float rotationSpeed = 0.0f;

		float orbitAngle = 0.0f;
		float orbitSpeed = 0.0f;
		float orbitRadius = 0.0f;
	};

	std::vector<Planet*> planets;

	class Moon : public Planet {
	public:
		void Update() override;

		Moon(
			Game* game,
			int type,
			float diameter,
			LPCWSTR texture,
			Planet* parentPlanet)
			: Planet(game,type, diameter, texture), parentPlanet(parentPlanet) {};

		Planet* parentPlanet;
	};

	std::vector<Moon*> moons;
};

