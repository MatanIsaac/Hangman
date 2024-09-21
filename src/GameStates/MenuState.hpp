#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"
#include <Graphics/TextRenderer.hpp>
#include <Graphics/Button.hpp>

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
	std::unique_ptr<Button, ButtonDeleter> m_PlayButton;	
	std::unique_ptr<Button, ButtonDeleter> m_QuitButton;	
	std::unique_ptr<TextRenderer, TextRendererDeleter> m_TextRenderer;
};
