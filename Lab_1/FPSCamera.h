#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent {
private:
	float fov;
	float aspectRatio;
	float nearPlane ;
	float farPlane;

	float cameraSensitivity = 0.02f;
	float cameraSpeed = 2.5f;

	Vector3 velocity = {0.0f, 0.0f, 0.0f};

	float cameraYaw = 0.0f;
	float cameraPitch = 0.0f;
public:

	FPSCamera(Game* game) : CameraComponent(game) {};

	void Initialize() override;
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

