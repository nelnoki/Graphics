#include "OrbitalCamera.h"
#include "Game.h"

void OrbitalCamera::Initialize(Vector3 orbitInput, Vector3 lookPointInput, Vector3 targetInput)
{
	orbit = orbitInput;

	lookPoint = lookPointInput;
	target = targetInput;
	upAxis = Vector3(0.0f, 1.0f, 0.0f);

	distance = Vector3::Distance(target, lookPoint);

	fov = 30.0f;
	aspectRatio = float(game->Display->ClientWidth) / float(game->Display->ClientHeight);
	nearPlane = 0.01f;
	farPlane = 100.0f;

	projMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	viewMatrix = Matrix::CreateLookAt(lookPoint, target, upAxis);

	cameraYaw = 0.0f;
	cameraPitch = -DirectX::XM_PIDIV4;

	Vector3 camPosition = Vector3(Vector3::Distance(target, lookPoint), 0.0f, 0.0f);
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(cameraYaw, 0, cameraPitch);
	orbit = Vector3::Transform(camPosition, rotationMatrix);
}

void OrbitalCamera::SetLookPoint(Vector3 lookPointInput) { lookPoint = lookPointInput; }
void OrbitalCamera::SetTarget(Vector3 targetInput) { target = targetInput; }
void OrbitalCamera::SetUpAxis(Vector3 upAxisInput) { upAxis = upAxisInput; }

void OrbitalCamera::HandleInput(float deltaTime)
{
	if (game->InDevice->IsKeyDown(Keys::RightButton))
	{
			Rotate(game->InDevice->MouseOffset);
	}

	if (game->InDevice->MouseWheelDelta != 0)
	{
		Zoom(game->InDevice->MouseWheelDelta * 0.1f);
	}
}

void OrbitalCamera::Zoom(float amount)
{
	distance -= amount;
	distance = distance > 5.0f ? distance : 5.0f;
}

void OrbitalCamera::Rotate(Vector2 mouseInput)
{
	/*Vector3 camPosition = Vector3(Vector3::Distance(target, lookPoint), 0.0f, 0.0f);

	cameraYaw += mouseInput.x * cameraSensitivity;
	cameraPitch -= mouseInput.y * cameraSensitivity;*/

	/*float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;
	if (cameraPitch < -pitchLimit) cameraPitch = -pitchLimit;
	if (cameraPitch > pitchLimit) cameraPitch = pitchLimit;

	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(cameraYaw, 0, cameraPitch);

	orbit = Vector3::Transform(camPosition, rotationMatrix);*/

	cameraYaw += mouseInput.x * cameraSensitivity;
	cameraPitch -= mouseInput.y * cameraSensitivity;

	const float pitchLimit = DirectX::XMConvertToRadians(89.0f);
	if (cameraPitch < -pitchLimit) cameraPitch = -pitchLimit;
	if (cameraPitch > pitchLimit) cameraPitch = pitchLimit;
}

void OrbitalCamera::Move(float deltaTime) {}

void OrbitalCamera::Update()
{
	//HandleInput(game->DeltaTime);

	lookPoint.x = target.x + distance * cosf(cameraPitch) * sinf(cameraYaw);
	lookPoint.y = target.y + distance * sinf(cameraPitch);
	lookPoint.z = target.z + distance * cosf(cameraPitch) * cosf(cameraYaw);

	projMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	viewMatrix = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void OrbitalCamera::Initialize() {}
void OrbitalCamera::Draw() {}
void OrbitalCamera::Reload() {}
void OrbitalCamera::DestroyResources() {}