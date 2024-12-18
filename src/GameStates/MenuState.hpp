#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"
#include "Core/IGame.hpp"
#include "Core/GameStateManager.hpp"
#include <Graphics/TextRenderer.hpp>
#include <Graphics/Button.hpp>

namespace isaac_hangman
{
	class MenuState : public IGameState
	{
	public:
		MenuState(GameStateManager& stateManager, IGame& game);
		virtual ~MenuState() override {}

		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render(SDL_Renderer* renderer) override;

	private:          
		GameStateManager&	m_GameStateManager;
		IGame& 				m_Game;
		Unique_Button 		m_PlayButton;	
		Unique_Button 		m_QuitButton;
	};
}