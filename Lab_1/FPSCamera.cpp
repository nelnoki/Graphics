#include "FPSCamera.h"
#include "Game.h"

void FPSCamera::Initialize()
{
	lookPoint = Vector3(0.0f, 0.0f, -1.0f);
	target = Vector3(0.0f, 0.0f, 0.0f);
	upAxis = Vector3(0.0f, 1.0f, 0.0f);

	fov = 30.0f;
	aspectRatio = float(game->Display->ClientWidth) / float(game->Display->ClientHeight);
	nearPlane = 0.01f;
	farPlane = 100.0f;

	projMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	viewMatrix = Matrix::CreateLookAt(lookPoint, target, upAxis);

	velocity = Vector3(0.0f, 0.0f, 0.0f);

	cameraYaw = atan2(target.z, -target.x);
	cameraPitch = asin(target.y);
}

void FPSCamera::SetLookPoint(Vector3 lookPointInput)
{
	lookPoint = lookPointInput;

	Vector3 direction;
	direction.x = -cos(cameraYaw) * cos(cameraPitch);
	direction.y = sin(cameraPitch);
	direction.z = cos(cameraPitch) * sin(cameraYaw);
	direction.Normalize();

	target = lookPoint + direction;
}

void FPSCamera::SetTarget(Vector3 targetInput)
{
	target = targetInput;

	Vector3 direction = target - lookPoint;
	direction.Normalize();

	cameraYaw = atan2(direction.z, -direction.x);
	cameraPitch = asin(direction.y);

	target = lookPoint + direction;
}

void FPSCamera::SetUpAxis(Vector3 upAxisInput)
{
	upAxis = upAxisInput;
}

void FPSCamera::HandleInput(float deltaTime)
{
	if (game->InDevice->IsKeyDown(Keys::RightButton)) 
		Rotate(game->InDevice->MouseOffset);

	Move(deltaTime);
}

void FPSCamera::Zoom(float amount)
{
	fov -= amount;
	fov = (fov < 10.0f) ? 10.0f : (fov > 90.0f) ? 90.0f : fov;
	projMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
}

void FPSCamera::Rotate(Vector2 mouseInput)
{
	cameraYaw += mouseInput.x * cameraSensitivity;
	cameraPitch += mouseInput.y * cameraSensitivity;

	float pitchLimit = DirectX::XM_PIDIV2 - 0.01f;
	if (cameraPitch < -pitchLimit) cameraPitch = -pitchLimit;
	if (cameraPitch > pitchLimit) cameraPitch = pitchLimit;

	Vector3 direction;
	direction.x = -cos(cameraYaw) * cos(cameraPitch);
	direction.y = sin(cameraPitch);
	direction.z = cos(cameraPitch) * sin(cameraYaw);
	direction.Normalize();

	target = lookPoint + direction;
}

void FPSCamera::Move(float deltaTime)
{
	Vector3 forward = target - lookPoint;
	forward.Normalize();

	Vector3 right = upAxis.Cross(forward);
	right.Normalize();

	float speed = 10.0f * deltaTime;

	if (game->InDevice->IsKeyDown(Keys::W)) velocity = forward * speed;
	if (game->InDevice->IsKeyDown(Keys::S)) velocity = -forward * speed;
	if (game->InDevice->IsKeyDown(Keys::A)) velocity = -right * speed;
	if (game->InDevice->IsKeyDown(Keys::D)) velocity = right * speed;

	if (!(game->InDevice->IsKeyDown(Keys::W)) &&
		!(game->InDevice->IsKeyDown(Keys::S)) &&
		!(game->InDevice->IsKeyDown(Keys::A)) &&
		!(game->InDevice->IsKeyDown(Keys::D)))
		velocity = Vector3(0.0f, 0.0f, 0.0f);

}


void FPSCamera::Update()
{
	//HandleInput(game->DeltaTime);

	lookPoint += velocity * cameraSpeed;
	target += velocity * cameraSpeed;
	projMatrix = Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane);
	viewMatrix = Matrix::CreateLookAt(lookPoint, target, upAxis);
}

void FPSCamera::Draw() {}
void FPSCamera::Reload() {}
void FPSCamera::DestroyResources() {}