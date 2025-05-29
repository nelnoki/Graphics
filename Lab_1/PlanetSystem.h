#pragma once
#include "GameComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"
#include "OrbitalCamera.h"
#include "FPSCamera.h"

class PlanetSystem : public GameComponent {
public:
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;

	PlanetSystem(
		Game* game, 
		int planetNum = 9, 
		std::vector<int> hasMoon = { } ) 
			: GameComponent(game), planetNum(planetNum), hasMoon(hasMoon) {};

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

	class Planet {
	public:
		void Update();

		Planet(Game* game) : game(game) {};

		Game* game;

		TriangleComponent* mesh;

		Vector3 pos = {0.0f, 0.0f, 0.0f};

		float rotationAngle = 0.0f;
		float rotationSpeed = 0.0f;

		float orbitAngle = 0.0f;
		float orbitSpeed = 0.0f;
		float orbitRadius = 0.0f;

		float diameter;
	};

	std::vector<Planet*> planets;

	class Moon : public Planet {
	public:
		void Update();

		Moon(Game* game) : Planet(game) {};

		Planet* parentPlanet;
	};

	std::vector<Moon*> moons;

	struct OrbitalCameraObject {
		OrbitalCamera* camera;
		Planet* cameraOwner;
	};

	std::vector<OrbitalCameraObject*> cameras;
	FPSCamera* mainFPS;
	CameraComponent* activeCam;

	void ChangeCamera(int cameraID);
};

