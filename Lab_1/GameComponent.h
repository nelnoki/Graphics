#pragma once

class Game;

class GameComponent {

public:
	GameComponent(Game* game);
	virtual ~GameComponent();

	Game* game;

	virtual void DestroyResources() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void Reload() = 0;
	virtual void Update() = 0;

};