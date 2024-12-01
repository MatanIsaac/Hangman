#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"
#include "Core/GameStateManager.hpp"
#include <Graphics/TextRenderer.hpp>
#include <Graphics/Button.hpp>

namespace isaac_hangman
{
	class MenuState : public IGameState
	{
	public:
		MenuState(GameStateManager& stateManager);
		~MenuState() override {}

		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render() override;

	private:
		GameStateManager& 	m_GameStateManager;
		UniqueButton 		m_PlayButton;	
		UniqueButton 		m_QuitButton;	
		//UniqueTextRenderer 	m_TextRenderer;
	};
}