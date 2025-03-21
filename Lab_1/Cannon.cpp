#include "Cannon.h"
#include "Game.h"

void Cannon::Initialize() {
	cannon = new TriangleComponent(game);
	cannon->Initialize();
	

	
}

void Cannon::Draw() {
	cannon->Draw();
	for (auto bullet : bullets) {
		bullet->Draw();
	}
}

void Cannon::Update() {

	if (game->InDevice->IsKeyDown(Keys::D)) {
		rotationAngle -= rotationSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::A)) {
		rotationAngle += rotationSpeed;

	}

	if (rotationAngle > 360.0f) rotationAngle -= 360.0f;

	DirectX::SimpleMath::Vector3 center(
		0.0f,
		0.0f,
		0.0f
	);

	DirectX::SimpleMath::Vector3 scale(
		0.05f,
		0.1f,
		0.0f
	);

	cannon->worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
		DirectX::SimpleMath::Matrix::CreateTranslation(center);

	cannon->worldMatrix = cannon->worldMatrix.DirectX::SimpleMath::Matrix::Transpose();

	if (game->InDevice->IsKeyDown(Keys::Space)) {

		TriangleComponent* bullet = new TriangleComponent
		(game,
			{
			DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f),
			DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)
			},
			{ 0,1,2 }
		);
		bullet->Initialize();

		bullet->worldMatrix =
			DirectX::SimpleMath::Matrix::CreateScale(Vector3(0.1f, 0.1f, 0.1f)) *
			DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
			DirectX::SimpleMath::Matrix::CreateTranslation(center);

		bullets.push_back(bullet);
	}

	for (auto bullet : bullets) {
		Vector3 velocity(
			shootSpeed * game->DeltaTime,
			shootSpeed * game->DeltaTime,
			0.0f
		);
		bullet->worldMatrix *=
			DirectX::SimpleMath::Matrix::CreateTranslation(velocity);

		bullet->worldMatrix = bullet->worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
	}
}

void Cannon::Reload() {
}

void Cannon::DestroyResources() {
	cannon->DestroyResources();
	for (auto& bullet : bullets) {
		bullet->DestroyResources();
		delete bullet;
	}
	bullets.clear();
}