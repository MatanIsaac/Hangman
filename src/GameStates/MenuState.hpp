#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"

class Game;


class MenuState : public IGameState
{
public:
	MenuState(Game* game);
	~MenuState() override;
	void ProcessInput() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer ) override;
	void Clean() override;

private:
	Game* m_Game;

};
