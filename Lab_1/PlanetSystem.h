#pragma once
#include "GameComponent.h"
#include "CubeComponent.h"
#include "SphereComponent.h"

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
		std::vector<int> hasMoon = { 1,2,3 } ) 
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

	// ��������� ����������� ������
	Vector3 cameraLookAt = Vector3::Zero;  // �����, ������ ������� ��������� ������
	float cameraDistance = 10.0f;         // ���������� �� ������
	float cameraYaw = DirectX::XM_PI;               // �������������� ���� (�������� ������ Y)
	float cameraPitch = -0.3f;             // ������������ ���� (������ �����/����)
	float cameraRoll = 0.0f;              // ������ ������ (������ 0)
	float cameraSensitivity = 0.005f;
	float zoomSpeed = 1.0f;

	Vector2 prevMousePos = Vector2::Zero;

	// ������ ��� ���������� �������
	void HandleCameraInput();
	void RotateCamera(float deltaYaw, float deltaPitch);
	void ZoomCamera(float deltaDistance);
};

