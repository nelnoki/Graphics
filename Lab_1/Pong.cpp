#include "Pong.h"
#include "Game.h"

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
	
	player1->Move(Keys::W, Keys::S, Keys::A, Keys::D, Keys::E, Keys::Q);
	player2->Move(Keys::I, Keys::K, Keys::J, Keys::L, Keys::U, Keys::O);
	ball->Move();

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
		if (ball->pos.x > 0.0f) {
			player1->score++;
		}

		else {
			player2->score++;
		}

		ResetBall();
			
		std::cout << "Score: " << player1->score << " - " << player2->score << "\n";
	}
}

//void Pong::HandlePaddleCollision(Player* player) {
//
//	float hitPosition = (ball->pos.y - player->pos.y) / player->scale.y;
//
//	float angle = hitPosition * MAX_BOUNCE_ANGLE;
//
//	float directionX = (player == player1) ? 1.0f : -1.0f;
//
//	ball->direction = Vector2(
//		directionX * cos(angle),
//		sin(angle)
//	);
//
//	ball->speed *= SPEED_INCREASE_FACTOR;
//}

void Pong::HandlePaddleCollision(Player* player) {
	Vector2 relativeHit = Vector2(ball->pos.x - player->pos.x, ball->pos.y - player->pos.y);

	Matrix inverseRot = Matrix::CreateRotationZ(-player->rotationAngle);
	Vector3 localHit3 = Vector3::Transform(Vector3(relativeHit.x, relativeHit.y, 0.0f), inverseRot);
	Vector2 localHit(localHit3.x, localHit3.y);

	float hitPosition = localHit.y / player->scale.y;
	if (hitPosition > 1.0f) hitPosition = 1.0f;
	else if (hitPosition < -1.0f) hitPosition = -1.0f;

	float angle = hitPosition * MAX_BOUNCE_ANGLE;

	float directionSign = (player == player1) ? 1.0f : -1.0f;
	Vector2 newDir = Vector2(directionSign * cos(angle), sin(angle));

	Matrix rot = Matrix::CreateRotationZ(player->rotationAngle);
	Vector3 finalDir3 = Vector3::Transform(Vector3(newDir.x, newDir.y, 0.0f), rot);
	Vector2 finalDir(finalDir3.x, finalDir3.y);
	finalDir.Normalize();

	ball->direction = finalDir;
	ball->speed *= SPEED_INCREASE_FACTOR;
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

void Pong::Player::Move(Keys up, Keys down, Keys left, Keys right, Keys rotateclock, Keys rotateanticlock) {
	if (game->InDevice->IsKeyDown(up)) pos.y += speed;
	
	if (game->InDevice->IsKeyDown(down)) pos.y -= speed;

	if (game->InDevice->IsKeyDown(right)) pos.x += speed;

	if (game->InDevice->IsKeyDown(left)) pos.x -= speed;

	if (game->InDevice->IsKeyDown(rotateclock)) rotationAngle += rotationSpeed;

	if (game->InDevice->IsKeyDown(rotateanticlock)) rotationAngle -= rotationSpeed;
	
	if (pos.y + scale.y > SCREEN_TOP) {
		pos.y = SCREEN_TOP - scale.y;
	}
	else if (pos.y - scale.y < SCREEN_BOTTOM) {
		pos.y = SCREEN_BOTTOM + scale.y;
	}

	worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(rotationAngle) *
		DirectX::SimpleMath::Matrix::CreateTranslation(pos);

	worldMatrix = worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
}

void Pong::Ball::Move() {
	pos.x += direction.x * speed * game->DeltaTime;
	pos.y += direction.y * speed * game->DeltaTime;
	

	if (pos.y + radius > SCREEN_TOP) {
		direction.y = -abs(direction.y); 
		pos.y = SCREEN_TOP - radius;
	}
	else if (pos.y - radius < SCREEN_BOTTOM) {
		direction.y = abs(direction.y); 
		pos.y = SCREEN_BOTTOM + radius;
	}

	if (abs(direction.x) < MIN_HORIZONTAL) {
		float signX = (direction.x > 0) ? 1.0f : -1.0f;
		float newY = direction.y / sqrt(direction.x * direction.x + direction.y * direction.y);

		direction.y = newY * sqrt(1.0f - MIN_HORIZONTAL * MIN_HORIZONTAL);
		direction.x = signX * MIN_HORIZONTAL;
	}

	worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(scale) *
		DirectX::SimpleMath::Matrix::CreateRotationZ(0.0f) *
		DirectX::SimpleMath::Matrix::CreateTranslation(pos);

	worldMatrix = worldMatrix.DirectX::SimpleMath::Matrix::Transpose();
}

