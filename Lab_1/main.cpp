#include "Game.h"
#include "TriangleComponent.h"
#include "Cannon.h"
#include "Pong.h"
#include "PlanetSystem.h"
#include "CubeComponent.h"


int main() {
	int screenWidth = 1000;
	int screenHeight = 1000;
	LPCWSTR name = L"Graphics";

	int ans;
	std::cout <<
		"Press \n" <<
		"1 - Cannon \n" <<
		"2 - Pong \n" <<
		"3 - Planet System \n" <<
		"4 - Cube \n";

	std::cin >> ans;

	Game* game = new Game(name, screenWidth, screenHeight);
	GameComponent* component;

	switch (ans) {
		case 1: 
		{
			component = new Cannon(game);
			break;
		}
			
		case 2:
		{
			component = new Pong(game);
			break;
		}

		case 3:
		{
			component = new PlanetSystem(game);
			break;
		}

		case 4:
		{
			component = new CubeComponent(game, 0.5);
			break;
		}
	}

	//game->Components.push_back(cannon);
	//game->Components.push_back(pong);
	//component = new PlanetSystem(game);

	game->Components.push_back(component);

	game->Run();
} 