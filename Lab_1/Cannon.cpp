#include "Cannon.h"
#include "Game.h"

void Cannon::Initialize() {
	cannon = new TriangleComponent(game);
	cannon->Initialize();
}

void Cannon::Draw() {
	cannon->Draw();
}

//void Cannon::Update() {
//	rotationSpeed = 0.06f;
//
//	if (game->InDevice->IsKeyDown(Keys::D)) {
//		rotationAngle -= rotationSpeed;
//	}
//
//	if (game->InDevice->IsKeyDown(Keys::A)) {
//		rotationAngle += rotationSpeed;
//
//	}
//
//	if (rotationAngle > 360.0f) rotationAngle -= 360.0f;
//
//	DirectX::SimpleMath::Vector3 center(
//		0.0f,
//		0.0f,
//		0.0f
//	);
//
//	DirectX::SimpleMath::Vector3 scale(
//		0.05f,
//		0.1f,
//		0.0f
//	);
//
//	cannon->worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
//		DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
//		DirectX::SimpleMath::Matrix::CreateTranslation(center);
//
//	cannon->worldMatrix = cannon->worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
//
//	if (game->InDevice->IsKeyDown(Keys::Space)) {
//		bullet = new TriangleComponent
//		(game,
//			{
//			DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//			DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
//			DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
//			},
//			{ 0,1,2 }
//		);
//		bullet->Initialize();
//
//		Vector3 localSpawnPoint = Vector3(
//			(cannon->points[0].x + cannon->points[3].x) / 2.0f, 
//		    cannon->points[0].y,
//			cannon->points[0].z);
//
//		Vector3::Transform(localSpawnPoint, cannon->worldMatrix, bulletPos);
//
//		bullet->worldMatrix =
//			Matrix::CreateScale(Vector3(0.1f, 0.1f, 0.0f)) *
//			Matrix::CreateRotationZ(rotationAngle) *
//			Matrix::CreateTranslation(bulletPos);
//
//		bullets.push_back(bullet);
//
//		game->Components.push_back(bullet);
//	}
//
//	//for (auto& bullet : bullets) {
//	//	bullet->worldMatrix =
//	//		Matrix::CreateScale(Vector3(0.1f, 0.1f, 0.0f)) *
//	//		Matrix::CreateRotationZ(0.0f) *
//	//		Matrix::CreateTranslation(Vector3(shootSpeed, shootSpeed, 0.0f));
//	//}
//}

void Cannon::Update() {
    rotationSpeed = 0.06f;

    // Handle rotation
    if (game->InDevice->IsKeyDown(Keys::D)) {
        rotationAngle -= rotationSpeed;
    }
    if (game->InDevice->IsKeyDown(Keys::A)) {
        rotationAngle += rotationSpeed;
    }
    rotationAngle = fmodf(rotationAngle, 360.0f);

    // Update cannon matrix
    cannon->worldMatrix = Matrix::CreateScale(Vector3(0.05f, 0.1f, 0.0f)) *
        Matrix::CreateRotationZ(rotationAngle) *
        Matrix::CreateTranslation(Vector3::Zero);

    cannon->worldMatrix = cannon->worldMatrix.Transpose();

    // Shooting logic
    if (game->InDevice->IsKeyDown(Keys::Space)) {
        // Calculate spawn point
        Vector3 localSpawnPoint(0.0f, 0.5f, 0.0f); // Top center of the cannon
        Vector3 worldSpawnPoint;
        Vector3::Transform(localSpawnPoint, cannon->worldMatrix, worldSpawnPoint);

        // Create bullet
        TriangleComponent* bullet = new TriangleComponent(
            game,
            {
                DirectX::XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
                DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
                DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f),  DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
            },
            { 0, 1, 2 }
        );
        bullet->Initialize();

        // Set initial bullet position and direction
        bullet->worldMatrix = Matrix::CreateScale(Vector3(0.1f)) *
            Matrix::CreateRotationZ(rotationAngle) *
            Matrix::CreateTranslation(worldSpawnPoint);

        bullets.push_back(bullet);
        game->Components.push_back(bullet);
    }

    // Update bullets
    for (auto it = bullets.begin(); it != bullets.end();) {
        // Calculate movement direction based on cannon's rotation
        Vector3 velocity = Vector3(
            cos(rotationAngle) * shootSpeed * game->DeltaTime,
            sin(rotationAngle) * shootSpeed * game->DeltaTime,
            0.0f
        );

        // Update bullet position
        (*it)->worldMatrix *= Matrix::CreateTranslation(velocity);

        // Remove old bullets (example condition)
        if ((*it)->worldMatrix.Translation().Length() > 10.0f) {
            game->Components.erase(std::remove(game->Components.begin(), game->Components.end(), *it), game->Components.end());
            delete* it;
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
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