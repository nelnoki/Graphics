#include "Pong.h"
#include "Game.h"
#include <algorithm>

void Pong::Initialize() {
	player1 = new Player(game);
	player1->Initialize();
	player1->pos = Vector3(
		-0.85f,
		0.0f,
		0.0f
	);

	player2 = new Player(game);
	player2->Initialize();
	player2->pos = Vector3(
		0.85f,
		0.0f,
		0.0f
	);

	ball = new Ball(game);
	ball->Initialize();
}

void Pong::Draw() {
	player1->Draw();
	player2->Draw();
	ball->Draw();
}

void Pong::Update() {
	
	player1->Update(Keys::W, Keys::S);
	player2->Update(Keys::Up, Keys::Down);
	ball->Update();

	auto ballSphere = ball->GetBoundingSphere();
	auto p1Box = player1->GetBoundingBox();
	auto p2Box = player2->GetBoundingBox();

	if (ballSphere.Intersects(p1Box)) {
		HandlePaddleCollision(player1);
	}


	if (ballSphere.Intersects(p2Box)) {
		HandlePaddleCollision(player2);
	}


	if (abs(ball->pos.x) > 1.05f) {
		ResetBall();
		
		if (ball->pos.x > 0) player1->score++;
		else player2->score++;
		std::cout << "Score: " << player1->score << " - " << player2->score << "\n";
	}
}

void Pong::HandlePaddleCollision(Player* player) {

	float hitPosition = (ball->pos.y - player->pos.y) / player->scale.y;
	hitPosition = abs(-1.0f - hitPosition) < abs(1.0f - hitPosition) ? -1.0f : 1.0f;


	float maxAngle = DirectX::XM_PIDIV2 * 0.75f; // ~67.5 
	float angle = hitPosition * maxAngle;


	float directionX = (player == player1) ? 1.0f : -1.0f;

	ball->direction = Vector2(
		directionX * cos(angle),
		sin(angle)
	);

	ball->speed *= 1.1f;
}

void Pong::ResetBall() {
	ball->pos = Vector3::Zero;
	ball->speed = 0.8f;
	ball->direction = Vector2(
		(rand() % 2 == 0) ? 1.0f : -1.0f,
		(rand() % 100 / 100.0f) - 0.5f
	);
}

void Pong::Reload() {
}

void Pong::DestroyResources() {
	player1->DestroyResources();
	player2->DestroyResources();
	ball->DestroyResources();
}

void Pong::Player::Update(Keys up, Keys down) {
	if (game->InDevice->IsKeyDown(up)) pos.y += speed;
	
	if (game->InDevice->IsKeyDown(down)) pos.y -= speed;

	const float screenTop = 1.0f;
	const float screenBottom = -1.0f;
	if (pos.y + scale.y > screenTop) {
		pos.y = screenTop - scale.y;
	}
	else if (pos.y - scale.y < screenBottom) {
		pos.y = screenBottom + scale.y;
	}

	worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(0.0f) *
		DirectX::SimpleMath::Matrix::CreateTranslation(pos);

	worldMatrix = worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
}

void Pong::Ball::Update() {
	pos.x += direction.x * speed * game->DeltaTime;
	pos.y += direction.y * speed * game->DeltaTime;

	const float screenTop = 1.0f;
	const float screenBottom = -1.0f;
	if (pos.y + radius > screenTop) {
		direction.y = -abs(direction.y); 
		pos.y = screenTop - radius;
	}
	else if (pos.y - radius < screenBottom) {
		direction.y = abs(direction.y); 
		pos.y = screenBottom + radius;
	}

	worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(0.0f) *
		DirectX::SimpleMath::Matrix::CreateTranslation(pos);

	worldMatrix = worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
}

