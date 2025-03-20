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
	float rotationSpeed = 0.0f;
	float rotationAngle = 0.0f;
	float shootSpeed = 3.0f;
	DirectX::SimpleMath::Vector3 bulletPos;
	DirectX::SimpleMath::Matrix worldMatrix;
	TriangleComponent* cannon;
	std::vector<TriangleComponent*> bullets;
	TriangleComponent* bullet;
};

