#pragma once
#include "GameComponent.h"
#include "BaseComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"
#include "CameraController.h"
#include "Planet.h"

class PlanetSystem : public GameComponent {
public:
	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Update() override;
	void Reload() override;

	PlanetSystem(
		Game* game,
		int planetNum = 10,
		std::vector<int> hasMoon = { 3, 5, 8, 9 })
		: GameComponent(game), planetNum(planetNum), hasMoon(hasMoon) {};

	std::vector<int> hasMoon;
	int planetNum;
	const wchar_t* texture_0 = L"Textures/sun.dds";
	const wchar_t* texture_1 = L"./Textures/mercury.dds";
	const wchar_t* texture_2 = L"./Textures/venus.dds";
	const wchar_t* texture_3 = L"./Textures/earth.dds";
	const wchar_t* texture_4 = L"./Textures/mars.dds";
	const wchar_t* texture_5 = L"./Textures/jupiter.dds";
	const wchar_t* texture_6 = L"./Textures/saturn.dds";
	const wchar_t* texture_7 = L"./Textures/uranus.dds";
	const wchar_t* texture_8 = L"./Textures/neptune.dds";
	const wchar_t* texture_9 = L"./Textures/ceres.dds";
	const wchar_t* texture_10 = L"./Textures/moon.dds";
	const wchar_t* texture_11 = L"./Textures/eris.dds";
	const wchar_t* texture_12 = L"./Textures/haumea.dds";
	const wchar_t* texture_13 = L"./Textures/makemake.dds";
	const wchar_t* texture_14 = L"./Textures/cube.dds";


	Matrix viewMatrix;
	Matrix projMatrix;

	CameraController* controller;

	std::vector<Planet*> planets;
	std::vector<Moon*> moons;
};

