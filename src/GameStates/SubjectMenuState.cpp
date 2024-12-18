#include "SubjectMenuState.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp" 
#include "GameStates/PlayState.hpp"
#include <iostream>

namespace isaac_hangman
{
	SubjectMenuState::SubjectMenuState(GameStateManager& stateManager, IGame& game)
		: m_GameStateManager(stateManager), m_Game(game), m_CurrentSubject(Subjects::NONE)
	{	
		glm::vec2 backButtonPosition( glm::vec2( 35.f, SCREEN_HEIGHT - 85.f ) );
		m_BackToMenuButton = std::make_unique<Button>("Back To Main Menu", backButtonPosition);

		glm::vec2 foodButtonPosition( glm::vec2( SCREEN_WIDTH / 3.f, ( SCREEN_HEIGHT / 3.f ) ) );
		m_SubjectFoodButton = std::make_unique<Button>("Food", foodButtonPosition);

		glm::vec2 countriesButtonPosition( glm::vec2( SCREEN_WIDTH / 3.f + 100, ( SCREEN_HEIGHT / 3.f ) ) );
		m_SubjectCountriesButton = std::make_unique<Button>("Countries", countriesButtonPosition);
	}

	void SubjectMenuState::ProcessInput()
	{
		if (m_SubjectFoodButton->isPressed())
		{
			m_CurrentSubject 		= Subjects::FOOD;
			const auto& randomWord 	= GetRandomWord(m_CurrentSubject);
			std::cout << "\n\tWord: " << randomWord << '\n';
			m_GameStateManager.PushState(std::make_shared<PlayState>(m_GameStateManager,randomWord,m_CurrentSubject,m_Game));
		}
		
		if (m_SubjectCountriesButton->isPressed())
		{
			m_CurrentSubject 		= Subjects::COUNTRIES;
			std::string randomWord 	= GetRandomWord(m_CurrentSubject);
			std::cout << "\n\tWord: " << randomWord << '\n';
			m_GameStateManager.PushState(std::make_shared<PlayState>(m_GameStateManager,randomWord,m_CurrentSubject,m_Game));
		}

		if(m_BackToMenuButton->isPressed())
		{
			m_GameStateManager.PopState();
		}
	}

	void SubjectMenuState::Update( float deltaTime )
	{ 
		m_BackToMenuButton->Update(deltaTime);
		m_SubjectFoodButton->Update(deltaTime);
		m_SubjectCountriesButton->Update(deltaTime);
	}

	void SubjectMenuState::Render(SDL_Renderer* renderer)
	{
		m_BackToMenuButton->Render(renderer);
		m_SubjectFoodButton->Render(renderer);
		m_SubjectCountriesButton->Render(renderer);

		auto& textRenderer = TextRenderer::GetInstance();
		textRenderer.RenderText(renderer,325, 25, COLOR_LIGHTORANGE, "Pick a Subject:");
	}
}