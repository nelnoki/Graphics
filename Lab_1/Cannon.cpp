#include "Cannon.h"
#include "Game.h"

void Cannon::Initialize() {
	cannon = new TriangleComponent(game);
	cannon->Initialize();

	bullet = new TriangleComponent
	(game, 
		{ 
		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) 
		},
		{ 0,1,2 }
	);
	bullet->Initialize();
	rotationSpeed = 0.0f;
	rotationAngle = 0.0f;
	shootSpeed = 0.0f;
	bulletPos = {
		0.0f,
		0.0f,
		0.0f
	};
}

void Cannon::Draw() {
	cannon->Draw();
}

void Cannon::Update() {
	rotationSpeed = 0.06f;

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
		/*bulletPos += rotationAngle * shootSpeed;*/

		bullet->worldMatrix =
			Matrix::CreateScale(Vector3(0.1f, 0.1f, 0.0f)) *
			Matrix::CreateRotationZ(rotationAngle) *
			Matrix::CreateTranslation(bulletPos);

		game->Components.push_back(bullet);

	}
}

void Cannon::Reload() {
}

void Cannon::DestroyResources() {
	cannon->DestroyResources();
}