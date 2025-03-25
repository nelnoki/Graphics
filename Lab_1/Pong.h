#pragma once

#include "TriangleComponent.h"
#include "GameComponent.h"
#include "Keys.h"
#include <DirectXCollision.h>

class Pong : public GameComponent {
public:
	Pong(Game* game) : GameComponent(game) {};

	void DestroyResources() override;
	void Draw() override;
	void Initialize() override;
	void Reload() override;
	void Update() override;

	static constexpr float SCREEN_TOP = 1.0f;
	static constexpr float SCREEN_BOTTOM = -1.0f;
	static constexpr float MAX_BOUNCE_ANGLE = DirectX::XM_PIDIV2 * 0.75f; // ~67.5 
	static constexpr float MIN_HORIZONTAL = 0.3f;

	class Player : public TriangleComponent {
	public:
		Player(
			Game* game,
			std::vector<DirectX::XMFLOAT4> points = {
				DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			},
			std::vector<int> indeces = { 0,1,2, 0,1,3 }
		)
			: TriangleComponent(
				game,
				points,
				indeces) {
		};

		float speed = 0.005f;
		int score = 0;

		DirectX::SimpleMath::Vector3 scale = Vector3(
			0.03f,
			0.1f,
			0.0f
		);

		DirectX::SimpleMath::Vector3 pos = Vector3(
			0.0f,
			0.0f,
			0.0f
		);

		void Move(Keys up, Keys down);

		DirectX::BoundingBox GetBoundingBox() const {
			Vector3 extents(scale.x, scale.y, 0.1f);
			return DirectX::BoundingBox(pos, extents);
		}
	};

	class Ball : public TriangleComponent {
	public:
		Ball(
			Game* game,
			std::vector<DirectX::XMFLOAT4> points = {
				DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
				DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)
			},
			std::vector<int> indeces = { 0,1,2, 0,1,3 }
		)
		: TriangleComponent(
			game,
			points,
			indeces) {};

		DirectX::SimpleMath::Vector2 direction = Vector2(
			(rand() % 2 == 0) ? 1.0f : -1.0f,
			(rand() % 100 / 100.0f) - 0.5f
		);
		float speed = 0.8f;
		float radius = 0.03f;

		DirectX::SimpleMath::Vector3 scale = Vector3(
			radius,
			radius,
			radius
		);

		DirectX::SimpleMath::Vector3 pos = Vector3(
			0.0f,
			0.0f,
			0.0f
		);

		void Move();

		DirectX::BoundingSphere GetBoundingSphere() const {
			return DirectX::BoundingSphere(pos, scale.x);
		}
	};

	Player* player1;
	Player* player2;
	Ball* ball;

	void HandlePaddleCollision(Player* player);
	void ResetBall();
};

