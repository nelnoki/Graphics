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

private:
	float rotationSpeed;
	float rotationAngle;
	float shootSpeed;
	DirectX::SimpleMath::Vector3 bulletPos;
	DirectX::SimpleMath::Matrix worldMatrix;
	TriangleComponent* cannon;
	TriangleComponent* bullet;
};

