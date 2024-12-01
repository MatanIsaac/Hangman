#include "MenuState.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"
#include "SubjectMenuState.hpp"
#include "Core/GameContext.hpp"

#include <glm/glm.hpp>
#include <iostream>

namespace isaac_hangman
{

	MenuState::MenuState(GameStateManager& stateManager)
		: m_GameStateManager(stateManager)
	{
		float xAxisCenter = SCREEN_WIDTH / 2.f;

		glm::vec2 playButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) ) );
		m_PlayButton = std::make_unique<Button>("Play", playButtonPosition);
		
		glm::vec2 quitButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) + 50.f ) );
		m_QuitButton = std::make_unique<Button>("Quit", quitButtonPosition); 

	}

	void MenuState::ProcessInput()
	{	
		if (m_PlayButton->isPressed())
		{		
			m_GameStateManager.PushState(std::make_shared<SubjectMenuState>(m_GameStateManager));
		}

		if (m_QuitButton->isPressed())
		{
			GameContext::GetInstance().m_IsRunning = false;
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

		auto& textRenderer = TextRenderer::GetInstance();
		textRenderer.RenderText(250, 100,COLOR_LIGHTORANGE,"Welcome To Hangman");

	}
}