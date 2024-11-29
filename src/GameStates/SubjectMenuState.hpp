#pragma once

#include <memory>
#include <SDL2/SDL.h>

#include "IGameState.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Core/GameStateManager.hpp"
#include "../Subjects.hpp"

namespace isaac_hangman
{
	class SubjectMenuState : public IGameState
	{
	public:
		SubjectMenuState(GameStateManager& stateManager);
		~SubjectMenuState() = default;

		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render() override;

	private:
		GameStateManager& 	m_GameStateManager;
		Subjects 			m_CurrentSubject;
		UniqueButton 		m_SubjectFoodButton;
		UniqueButton 		m_SubjectCountriesButton;
		UniqueButton 		m_BackToMenuButton;
		UniqueTextRenderer 	m_TextRenderer;
	};
}