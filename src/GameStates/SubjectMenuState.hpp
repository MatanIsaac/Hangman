#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include "IGameState.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Core/GameStateManager.hpp"
#include "Core/IGame.hpp"
#include "../Subjects.hpp"

namespace isaac_hangman
{
	class SubjectMenuState : public IGameState
	{
	public:
		SubjectMenuState(GameStateManager& stateManager, IGame& game);
		virtual ~SubjectMenuState() override {}

		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render(SDL_Renderer* renderer) override;

	private:
		GameStateManager& 	m_GameStateManager;
		IGame& 				m_Game;
		SDL_Renderer* 		m_Renderer;
		Subjects 			m_CurrentSubject;
		Unique_Button 		m_SubjectFoodButton;
		Unique_Button 		m_SubjectCountriesButton;
		Unique_Button 		m_BackToMenuButton;
	};
}