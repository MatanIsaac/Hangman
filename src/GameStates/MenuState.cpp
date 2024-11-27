#include "MenuState.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"

#include <glm/glm.hpp>
#include <iostream>
#include "SubjectMenuState.hpp"

namespace isaac_hangman
{

	MenuState::MenuState(GameStateManager& stateManager)
		: m_GameStateManager(stateManager)
	{
		float xAxisCenter = SCREEN_WIDTH / 2.f;
		glm::vec2 playButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) ) );
		m_PlayButton.reset(new Button("Play", 36, playButtonPosition));
		
		glm::vec2 quitButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) + 50.f ) );
		m_QuitButton.reset(new Button("Quit", 36, quitButtonPosition)); 

		m_TextRenderer = std::make_unique<TextRenderer>(40); 
	}

	void MenuState::ProcessInput()
	{	
		if (m_PlayButton->isPressed())
		{		
			m_GameStateManager.PushState(std::make_shared<SubjectMenuState>(m_GameStateManager));
		}

		if (m_QuitButton->isPressed())
		{
				
		}
	}

	void MenuState::Update( float deltaTime )
	{
		m_PlayButton->Update(deltaTime);
		m_QuitButton->Update(deltaTime);
	}

	void MenuState::Render()
	{
		m_PlayButton->Render();
		m_QuitButton->Render();
		m_TextRenderer->RenderText(250, 100,COLOR_LIGHTORANGE,"Welcome To Hangman");

	}
}