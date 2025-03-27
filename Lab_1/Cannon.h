#pragma once

#include "TriangleComponent.h"
#include "GameComponent.h"

class Cannon : public GameComponent {
public:
	
	Cannon(Game* game) : GameComponent(game) {};

	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Reload() override;
	void Update() override;

	TriangleComponent* cannon;
	float rotationSpeed = 0.02f;
	float movingSpeed = 0.005f;
	float rotationAngle = DirectX::XM_PIDIV2;

	float shootInterval = 0.2f;
	float lastShotTime = -1.0f;
	float shootSpeed = 0.8f;
	
	DirectX::SimpleMath::Vector3 pos = Vector3(
		0.0f,
		0.0f,
		0.0f
	);

	DirectX::SimpleMath::Vector3 scale = Vector3(
		0.15f,
		0.075f,
		0.0f
	);

	class Bullet : public TriangleComponent {
	public:
		Bullet(
			Game* game,
			std::vector<DirectX::XMFLOAT4> points = {
				DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			std::vector<int> indeces = { 0,1,2 } 
		)
		: TriangleComponent(
			game,
			points,
			indeces) {};

		DirectX::SimpleMath::Vector2 direction;

		DirectX::SimpleMath::Vector3 scale = Vector3(
			0.05f,
			0.05f,
			0.0f
		);
	};

	std::vector<Bullet*> bullets;
};

