#include "SubjectMenuState.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp" 
#include "GameStates/PlayState.hpp"
#include <iostream>

namespace isaac_hangman
{
	SubjectMenuState::SubjectMenuState(GameStateManager& stateManager)
		: m_GameStateManager(stateManager)
	{	
		m_CurrentSubject = Subject::Subjects::NONE;

		glm::vec2 foodButtonPosition( glm::vec2( SCREEN_WIDTH / 3.f, ( SCREEN_HEIGHT / 3.f ) ) );
		m_SubjectFoodButton.reset(new Button("FOOD", 36, foodButtonPosition));

		glm::vec2 countriesButtonPosition( glm::vec2( SCREEN_WIDTH / 3.f + 100, ( SCREEN_HEIGHT / 3.f ) ) );
		m_SubjectCountriesButton.reset(new Button("COUNTRIES", 36, countriesButtonPosition));

		m_TextRenderer = std::make_unique<TextRenderer>(40);
	}

	void SubjectMenuState::ProcessInput()
	{
		if (m_SubjectFoodButton->isPressed())
		{
			m_CurrentSubject = Subject::Subjects::FOOD;
			std::string randomWord = Subject::GetRandomWord(m_CurrentSubject);
			m_GameStateManager.SetState(std::make_shared<PlayState>(m_GameStateManager,randomWord,m_CurrentSubject));
		}
		
		if (m_SubjectCountriesButton->isPressed())
		{
			m_CurrentSubject = Subject::Subjects::COUNTRIES;
			std::string randomWord = Subject::GetRandomWord(m_CurrentSubject);
			m_GameStateManager.SetState(std::make_shared<PlayState>(m_GameStateManager,randomWord,m_CurrentSubject));
		}
	}

	void SubjectMenuState::Update( float deltaTime )
	{ 
		m_SubjectFoodButton->Update(deltaTime);
		m_SubjectCountriesButton->Update(deltaTime);
	}

	void SubjectMenuState::Render()
	{
		m_SubjectFoodButton->Render();
		m_SubjectCountriesButton->Render();
		m_TextRenderer->RenderText
		(
			250, 
			100,
			COLOR_LIGHTORANGE,
			"Pick a Subject:"
		);
	}
}