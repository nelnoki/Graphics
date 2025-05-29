#pragma once
#include "CameraComponent.h"

class OrbitalCamera : public CameraComponent {
private:
	float fov;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	float cameraSensitivity = 0.005f;
	float cameraSpeed = 0.5f;

	float distance;

	float cameraYaw;
	float cameraPitch;

	void Initialize() override;

public:
	Vector3 orbit;

	OrbitalCamera(Game* game) : CameraComponent(game) {};

	void Initialize(Vector3 orbitInput, Vector3 lookPointInput, Vector3 targetInput);

	void Update() override;
	void DestroyResources() override;
	void Draw() override;
	void Reload() override;

	void SetLookPoint(Vector3 lookPointInput);
	void SetTarget(Vector3 targetInput);
	void SetUpAxis(Vector3 upAxisInput);

	void HandleInput(float deltaTime);
	void Rotate(Vector2 mouseInput);
	void Move(float deltaTime);
	void Zoom(float amount);

};