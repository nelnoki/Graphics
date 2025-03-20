#include "Game.h"
#include "TriangleComponent.h"
#include "Cannon.h"

int main() {
	int screenWidth = 800;
	int screenHeight = 800;

	Game* game = Game::GetInstance();
	game->Initialize(L"Lab1", screenWidth, screenHeight);
	
	LPCWSTR shaderURL = L"./Shaders/MyVeryFirstShader.hlsl";

	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),

		DirectX::XMFLOAT4(-0.9f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.9f, 0.3f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.85f, 0.3f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(-0.85f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),

		DirectX::XMFLOAT4(0.9f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.9f, 0.3f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.85f, 0.3f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.85f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	std::vector<int> indeces = { 0,1,2, 0,1,3 /*4,5,6, 4,6,7, 8,9,10, 8,11,10*/};

	Cannon* cannon = new Cannon(game);

	game->Components.push_back(cannon);

	game->Run();

}