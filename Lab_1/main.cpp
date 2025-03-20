#include "Game.h"
#include "TriangleComponent.h"

int main() {
	int screenWidth = 800;
	int screenHeight = 800;

	Game* game = Game::GetInstance();
	game->Initialize(L"Lab1", screenWidth, screenHeight);
	
	LPCWSTR shaderURL = L"./Shaders/MyVeryFirstShader.hlsl";

	std::vector<DirectX::XMFLOAT4> points = {
		DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.5f, 0.5f, 1.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f),
	};

	std::vector<int> indeces = { 0,1,2, 1,0,3, 3,0,4};

	TriangleComponent* triangle = new TriangleComponent(game, shaderURL, points, indeces);

	game->Components.push_back(triangle);

	game->Run();

}