#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"

class Game;

class SubjectMenuState : public IGameState
{
public:
    SubjectMenuState(Game* game);
    ~SubjectMenuState() override;

	void ProcessInput() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer ) override;
	void Clean() override;

private:
	Game* m_Game;

};
