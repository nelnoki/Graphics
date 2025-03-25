#include "Game.h"
#include "TriangleComponent.h"
#include "Cannon.h"
#include "Pong.h"

int main() {
	int screenWidth = 1000;
	int screenHeight = 1000;

	Game* game = Game::GetInstance();
	game->Initialize(L"Graphics", screenWidth, screenHeight);

	Cannon* cannon = new Cannon(game);
	Pong* pong = new Pong(game);

	int ans;
	std::cout <<
		"Press \n" <<
		"1 - Cannon \n" <<
		"2 - Pong \n";
	std::cin >> ans;
	switch (ans) {
		case 1: 
		{
			game->Components.push_back(cannon);
			break;
		}
			
		case 2:
		{
			game->Components.push_back(pong);
			break;
		}
	}
	game->Run();
}