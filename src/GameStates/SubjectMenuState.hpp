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
		~SubjectMenuState() override {};

		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render() override;

	private:
		GameStateManager& m_GameStateManager;

		std::unique_ptr<Button> m_SubjectFoodButton;
		std::unique_ptr<Button> m_SubjectCountriesButton;
		std::unique_ptr<Button> m_BackToMenuButton;
		std::unique_ptr<TextRenderer> m_TextRenderer;
		Subject::Subjects m_CurrentSubject;
	};
}