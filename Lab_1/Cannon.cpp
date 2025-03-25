#include "Cannon.h"
#include "Game.h"
#include <cmath>

void Cannon::Initialize() {
	cannon = new TriangleComponent(game, 
		{
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
		},
		{ 0,1,2, 0,1,3 });

	cannon->Initialize();
}

void Cannon::Draw() {
	cannon->Draw();
	for (auto bullet : bullets) {
		bullet->Draw();
	}
}

void Cannon::Update() {

	if (game->InDevice->IsKeyDown(Keys::E)) {
		rotationAngle -= rotationSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::Q)) {
		rotationAngle += rotationSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::W)) {
		cannonPos.y += movingSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::S)) {
		cannonPos.y -= movingSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::D)) {
		cannonPos.x += movingSpeed;
	}

	if (game->InDevice->IsKeyDown(Keys::A)) {
		cannonPos.x -= movingSpeed;
	}

	cannon->worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(cannonScale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
		DirectX::SimpleMath::Matrix::CreateTranslation(cannonPos);

	cannon->worldMatrix = cannon->worldMatrix.DirectX::SimpleMath::Matrix::Transpose();

	if (game->InDevice->IsKeyDown(Keys::Space)) {

		if (std::abs(game->TotalTime - lastShotTime) >= shootInterval) {
			Bullet* bullet = new Bullet(game);

			bullet->Initialize();

			bullet->direction = Vector2(
				cos(rotationAngle),
				sin(rotationAngle)
			);

			DirectX::SimpleMath::Vector3 localGunEnd(1.0f, 0.0f, 0.0f);

			DirectX::SimpleMath::Vector3 bulletPos =
				DirectX::SimpleMath::Vector3::Transform(localGunEnd, cannon->worldMatrix.DirectX::SimpleMath::Matrix::Transpose());

			bullet->worldMatrix =
				DirectX::SimpleMath::Matrix::CreateScale(bulletScale) *
				DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
				DirectX::SimpleMath::Matrix::CreateTranslation(bulletPos);

			bullet->worldMatrix = bullet->worldMatrix.DirectX::SimpleMath::Matrix::Transpose();

			bullets.push_back(bullet);
			lastShotTime = game->TotalTime;
		}
	}

	for (Bullet* bullet : bullets) {
		bullet->worldMatrix._14 += bullet->direction.x * shootSpeed * game->DeltaTime;
		bullet->worldMatrix._24 += bullet->direction.y * shootSpeed * game->DeltaTime;
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